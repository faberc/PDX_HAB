//
//  main.h
//  APRS Radio
//
//  Created by Chuck Faber on 6/7/18.
//  Copyright © 2018 Portland State University. All rights reserved.
//

#ifndef main_h
#ifndef ACEHAB_h
#define main_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

struct telem {
    char * latitude;
    char * NS;
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

extern TELEM t_data;

/* Function Prototypes */
#include "radiomod.h"
#include "I2Csensors.h"
#include "aprs.h"
#include "gps.h"

#endif /* end the ifndef ACEHAB_h */
#endif /* main_h */
