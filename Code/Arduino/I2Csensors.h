//
//  I2Csensors.h
//  APRS Radio
//
//  Created by Abdulrahman Alamar on 6/7/18.
//  Copyright © 2018 Portland State University. All rights reserved.
//

#ifndef I2Csensors_h
#define I2Csensors_h

#include "main.h"
#else
#include "ACEHAB.h"
#endif

/* Function Prototypes */

/* Initializes the I2C sensors */
void init_sensors();

/* Reads the temperature */
void read_temp(char *temp);

/* Reads the pressure and altitude */
void read_press_alt (char *alt, char *press);


#endif /* I2Csensors_h */
