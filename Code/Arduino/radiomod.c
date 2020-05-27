//
//  ACEHAB.c
//  APRS Radio
//
//  Created by Chuck Faber on 6/09/18.
//  Copyright © 2018 Portland State University. All rights reserved.
//
//  ECE 103 - Team 03: Abdulrahman Alamar, Chuck Faber, Ebtehal Alenzi
//
//  This is the script which generates the sine wave and modulates the frequency to send out the bits.
//
//  For function description overview please see associated header file.

#include "radiomod.h"

/* Global Declarations */
// Sample Rate = [Frequency] * [# of Samples (64)]

/* Timer 1 output compare values to generate frequencies */
unsigned long long int sample_rate_1200 = 76800;
unsigned long long int sample_rate_2200 = 140800;

unsigned char freq_1200 = F_CPU / sample_rate_1200; // Comparator values for 1200Hz and 2200 Hz.
unsigned char freq_2200 = F_CPU / sample_rate_2200;

/* Timer 2 output compare value - sends bits at 1200 Hz */
// Clock Speed / Prescaler / Desired Frequency
unsigned char encoding_rate_1200hz = F_CPU / 64 / 1200;

static unsigned char freq = freq_1200; // The PWM comparator value to generate appropriate frequencies

/* Sine Table Generated from: http://www.daycounter.com/Calculators/Sine-Generator-Calculator.phtml */
const unsigned char sine_wave[64] PROGMEM = {
    0x80,0x8c,0x98,0xa5,0xb0,0xbc,0xc6,0xd0,
    0xda,0xe2,0xea,0xf0,0xf5,0xfa,0xfd,0xfe,
    0xff,0xfe,0xfd,0xfa,0xf5,0xf0,0xea,0xe2,
    0xda,0xd0,0xc6,0xbc,0xb0,0xa5,0x98,0x8c,
    0x80,0x73,0x67,0x5a,0x4f,0x43,0x39,0x2f,
    0x25,0x1d,0x15,0xf,0xa,0x5,0x2,0x1,
    0x0,0x1,0x2,0x5,0xa,0xf,0x15,0x1d,
    0x25,0x2f,0x39,0x43,0x4f,0x5a,0x67,0x73,
};

const int sinewave_length = 64;

int outputPin = 13;
volatile uint16_t sample;

void waveGenerate (void) {
    pinMode(outputPin, OUTPUT); // Output waveform on pin 13
    TCCR0A |= _BV(WGM01) | _BV(WGM00);    // Set Timer 0 to FastPWM mode
    TCCR0B &= ~_BV(WGM02);    // Set WGM02 to 0
    
    TCCR0A = (TCCR0A | _BV(COM0A1)) & ~_BV(COM0A0); // Set to clear timer 0 compare value when duty cycle matches, set back at bottom
    TCCR0A &= ~(_BV(COM0B1) | _BV(COM0B0)); // Disconnect OC0B - normal port operation
    
    TCCR0B = (TCCR0B & ~(_BV(CS02) | _BV(CS01))) | _BV(CS00); // Set timer 0 to have no prescaler (001)
    OCR0A = pgm_read_byte(&sine_wave[0]);   //Set PWM compare value for timer 1 to first element in sine table
    
    cli();    // Disable all interrupts while setting up timer 1
    
    TCCR1B = (TCCR1B & ~_BV(WGM13)) | _BV(WGM12);   // Set to CTC mode
    TCCR1A = TCCR1A & ~(_BV(WGM11) | _BV(WGM10));   // 0100
    
    TCCR1B = (TCCR1B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10); // No prescaler for timer 1 001
    
    OCR1A = freq;
    
    TIMSK1 |= _BV(OCIE1A);  // Initialize Timer 1 Interrupt Sequence Routine
    sample = 0;   // Initialize Sine Table Index Sample to 0
    
    
    TCCR2B = (TCCR2B & ~_BV(WGM20)) | _BV(WGM21);   // Set Timer 2 to CTC mode -010
    TCCR2B = (TCCR2B & ~_BV(WGM22));   // Set WGM22 to zero
    TCCR2B = (TCCR2B & ~(_BV(CS20) | _BV(CS21))) | _BV(CS22); // Prescaler of 64: 100
    TCNT2 = 0x00;   // Set counter to 0
    OCR2A = encoding_rate_1200hz; // Send out bits at 1200 Hz.
    
    sei();    // Enable all interrupts
    
    
}

/* More Global Declarations */
unsigned char buffer[256];
unsigned char buff_tail = 0;
unsigned char buff_head = 0;


bool is_bufferempty (void) {
    return(buff_tail == buff_head);
}

unsigned char getfrom_buffer (void) {
    unsigned char c;
    if (is_bufferempty()) {
        c = 0x7E;
    } else {
        c = buffer[buff_head];
        ++buff_head;
    }
    return c;
}

/* Adds the completed AX.25 string to the buffer variable and returns the index of the buffer tail (last character) */
int buffer_ax25packet ( const char* s ) {
    buff_head = 0;
    memset(buffer, 0, 256);
    strcpy(buffer, s);
    buff_tail = strlen(buffer) - 1;
    return buff_tail;
}

/* This interrupt triggers at the rate defined above to vary the frequency. It changes the Timer 0 compare value within it so that the duty cycle gets varied. */
ISR(TIMER1_COMPA_vect) {
    if (sample >= sinewave_length) {
        sample = -1;  // If the sample is over the length of the table, reset the value
    }
    else {
        OCR0A = pgm_read_byte(&sine_wave[sample]);  // Set the compare value for timer 0 to a value from sine table
        
    }
    sample += 1;
    OCR1A = freq;   // Switch to currently assigned frequency.
    
}

ISR(TIMER2_COMPA_vect) {
    unsigned char tt_index = 0;
    static unsigned char tone_table[2] = {freq_1200, freq_2200};
    static uint8_t ones = 0;
    static bool sending_flag;
    static unsigned char c;
    static uint8_t bits = 0;
    
    if (bits == 0) {
        if (is_bufferempty()) {
            return;
        }
        c = getfrom_buffer();
        bits = 8;
        sending_flag = (c == 0x7E);
    }
    
    if (!(byte & 0x01)) {           // If LSB is not 1 (0)
        tt_index = !tt_index;       // Switch frequency
        freq = tone_table[tt_index];
        ones = 0;                   // Reset ones counter to zero
        
        c = c >> 1;           // Shift to next bit in the character
        bits--;
    } else if ((ones++ >= 5) && !sending_flag) {   // If there are 5 or more 1s in a row and not sending the AX25 flag
        tt_index = !tt_index;   // Switch the frequency to send a zero
        freq = tone_table[tt_index];
        ones = 0;               // Set ones count back to zero.
        
        c = c >> 1;
        bits--;
        
    } else {          // the bit is a 1 and it's not a flag, so don't change freq and count ones up by one
        c = c >> 1;
        bits--;
        ones++;
    }
}
