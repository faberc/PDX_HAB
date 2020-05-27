//
//  radiomod.h
//  APRS Radio
//
//  Created by Chuck Faber on 6/7/18.
//  Copyright © 2018 Portland State University. All rights reserved.
//

#ifndef radiomod_h
#define radiomod_h

#include "main.h"
#else
#include "ACEHAB.h"
#endif

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#define delay_correct 64    // Delay and Millis() correction for prescalar of 1 on TCCR0B

/* Function Prototypes */

/* Initializes all the timers and starts the waveform generation at the defined frequency */
void waveGenerate (void);

/* Checks to see if tail index is the same as the head index to tell if the transmission buffer is empty */
unsigned char is_bufferempty (void);

/* Gets a character from the buffer for transmission */
unsigned char getfrom_buffer (void);

/* Copies the AX.25 packet into the transmission buffer */
void buffer_ax25packet ( const char* s );


#endif /* radiomod_h */
