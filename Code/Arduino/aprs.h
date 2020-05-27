//
//  aprs.h
//  APRS Radio
//
//  Created by Chuck Faber on 5/31/18.
//  Copyright © 2018 Portland State University. All rights reserved.
//



#ifndef aprs_h
#define aprs_h

#ifndef ACEHAB_h
#include "main.h"
#else
#include "ACEHAB.h"
#endif

/* Function Prototypes */

/* Constructs the SSID in the correct bit format */
unsigned char ssidcalc ( unsigned char ssid, unsigned char is_last);

/* Formats callsigns in the address segment correctly */
void callsignstr (char *ax25callsign);

/* Finds the correct via using the altitude and formats it in AX.25 */
void viastr (char *via, double altitude);

/* Calculates the CRC for the entire packet string */
unsigned short int crcCalc (const char *s);

/* Constructs the data into the predefined (by us) format */
void constructData (char* datastr, TELEM t_data, char* message);

/* Constructs the address in the correct AX.25 Format */
void constructAddress (char* address, double altitude);

/* Generates the entire packet string */
void generate_ax25packet (char* s, TELEM t_data, char* message);


#endif /* aprs_h */
