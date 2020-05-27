//
//  deprecated-gps.c
//  APRS Radio
//
//  Created by Ebtehal Alenzi on 6/12/18.
//  Copyright © 2018 Portland State University. All rights reserved.
//

#include <stdio.h>
#include <SoftwareSerial.h>
#include <string.h>

SoftwareSerial GPS(4,3);
char strc[2] = "c";
char str[200];

char gprmc[256];
char gmtTime[11];
char positionData[2];
char longitude[12];
char north[2];
char latitude[12];
char west[2];


void setup()
{
    Serial.begin (115200);
    GPS.begin (9600);
    //memset(str, 0, 200);
}

void loop()
{
    if (GPS.available()) {
        strc[0] = GPS.read();
        while (strc[0] != '\n') {
            strcat(str, strc);
            strc[0] = GPS.read();
            Serial.write(str);
            Serial.print("\n");
        }
    }
    
    
    //    if (str[3] == 'R') {
    //      Serial.print("\n\n\nTHIS IS A STRING: ");
    //      strcpy(gprmc, str);
    //      Serial.write(gprmc);
    //sscanf(gprmc, "$GPRMC,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]", gmtTime, positionData, longitude, north, latitude, west);
    
    //      Serial.print("\nGMTtime: ");
    //      Serial.write(gmtTime);
    //      Serial.print("\npositionData: ");
    //      Serial.write(positionData);
    //      Serial.print("\nlongitude: ");
    //      Serial.write(longitude);
    //      Serial.print("\nNorth: ");
    //      Serial.write(north);
    //      Serial.print("\nlatitude: ");
    //      Serial.write(latitude);
    //      Serial.print("\nWest: ");
    //      Serial.write(west);
    
    //      delay(1000);
    //
    //      memset(gprmc, 0, 256);
    //      memset(gmtTime, 0, 11);
    //      memset(positionData, 0, 2);
    //      memset(longitude, 0, 12);
    //      memset(north, 0, 2);
    //      memset(latitude, 0, 12);
    //      memset(west, 0, 2);
    //  }
    
    //delay(500);
    // memset(str, 0, 200);
    }
