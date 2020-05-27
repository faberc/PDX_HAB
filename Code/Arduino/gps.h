//
//  gps.h
//  APRS Radio
//
//  Created by Team 03 on 6/7/18.
//  Copyright © 2018 Portland State University. All rights reserved.
//

#ifndef gps_h
#define gps_h

#ifndef ACEHAB_h
#include "main.h"
#else
#include "ACEHAB.h"
#endif

/* Sets up the serial ports for reading the GPS data */
void setupGPS(void);

/* Reads the GPS data bit by bit into a buffer */
void getGPSdata(void);

/* Reads the gotten string to see if it is the appropriate NMEA sentence */
void readNMEA(void);

/* Processes the sentence if it is of form GGA */
void processGGA(void);

/* If the fix is valid, it saves the data into the global structure */
void saveGPSdata(void);


#endif /* gps_h */
