//
//  ACEHAB.h
//  APRS Radio
//
//  Created by Chuck Faber on 6/11/18.
//  Copyright © 2018 Portland State University. All rights reserved.
//

#ifndef ACEHAB_h
#define ACEHAB_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

struct telem {
    char * latitude;
    char * NW;
    char * longitude;
    char * EW;
    char * gps_alt;
    char * gps_time;
    float temperature;
    float pressure;
    float altitude;
    char * humidity;
    char * num_satellites;
};

typedef struct telem TELEM;

/* Defines the global telemetry structure */
extern TELEM t_data;

/* Function Prototypes */
#include "gps.h"
#include "I2Csensors.h"
#include "aprs.h"
#include "radiomod.h"

#endif /* ACEHAB_h */
