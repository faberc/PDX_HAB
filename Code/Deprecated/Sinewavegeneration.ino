//
//  radiomod.c
//  APRS Radio
//
//  Created by Chuck Faber on 6/5/18.
//  Copyright © 2018 Portland State University. All rights reserved.
//
//  This function generates a sine wave and also modulates the frequency of the wave to send out bits to the APRS network


#include <avr/interrupt.h>
#include <avr/io.h>
//#include "radiomod.h"

#define CLOCKFREQ 16000000

/* Define Interrupt Timer Frequencies */
    /* Clock Speed: 16MHz / 8 pre-scalar / (1200Hz * 32 Samples) */
#define HZ_1200 52     // produces ~1202Hz -> 0.16% error
    /* Clock Speed: 16MHz / 8 pre-scalar / (2200Hz * 32 Samples) */
//#define HZ_2200 25     // produces ~2203Hz -> 0.13% error

/* Define encoding timer */
    /* 16MHz / 64 prescalar / 1200 - 1 = 207.3 */
//#define TIMER_1200 191/2


unsigned char output_freq = 52; //CLOCKFREQ / 8 / (1200*32) / 2; // Initialize Output Frequency

/* Sine Table */
unsigned char sinewave_index = 0;
unsigned char sinewave[32] = {
    254,  25,  13,  99, 155, 151, 175,  63,
    255,  63, 175, 151, 155,  99,  13,  25,
    254, 102, 114,  28, 164, 168, 144,  64,
    0,  64, 144, 168, 164,  28, 114, 102
}; 


void radiomod_init (void) {
    /* Enable Timer Compares */
    TIMSK1 &= (1<<OCIE1A);
    
    /* Initialize Port (radio) output */
    DDRB = DDRB | 0xFF;     // Initialize port B data direction register as all output.
    PORTB = 0x00;           // Initialize all Port B values to 0.

    //pinMode(11, OUTPUT);

    /* Initialize Timers */
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    
    
    /* Waveform generation timer */
    TCCR1A &= ~(1<<WGM10);    // Clear WGM10
    TCCR1A &= ~(1<<WGM11);    // Clear WGM12
    TCCR1B |= (1<<WGM12);     // Set WGM10

    TCCR1A &= ~(1<<COM1A1);   // Clear COM1A1 to 0
    TCCR1A |= (1<<COM1A0);    // Set COM1A0 to 1
    
    TCCR1B |= (1<<CS12);     // Init timer 2 with prescalar 8
    TCNT1 = 0x00;           // Initialize counter to 0
    OCR1A = output_freq;    // Set output compare value to calculated value to generate particular wave freq.
//
//    /* Transmission encoding */
//    TCCR3B = TCCR3B | (1 >> WGM32); // ??
//    TCCR3B |= ((1<<CS31)|(1<<CS30));     // Init timer 3 with prescalar 1
//    TCNT3 = 0x00;
//    OCR3A = TIMER_1200;
}

/* Set up buffers */

//unsigned char is_buff_empty(void) {
//    return (buffer_head == buffer_tail);
//}

//unsigned char get_byte(void) {
//    unsigned char next_char;
//    
//    if (is_buff_empty() == 1) {
//        next_char = AX25_FLAG;
//    }
//}

/* Interrupt Sequences */

ISR(TIMER1_COMPA_vect) {
    static unsigned char sinewave_index = 0; // Intialize table index
    //PORTB = sinewave[sinewave_index++];
    OCR1A = output_freq;
    sinewave_index &= 0x1f;
}

//ISR(TIMER3_OVF_VECT) {
//    
//}

void setup () {
  radiomod_init();
  Serial.begin(9600);
}

void loop() {
  Serial.write("Test\n");
}
