//  gps.c
//  APRS Radio
//
//  Created by Ebtehal Alenzi and Chuck Faber on 6/7/18.
//  Copyright © 2018 Portland State University. All rights reserved.
//
//  ECE 103 - Team 03: Abdulrahman Alamar, Chuck Faber, Ebtehal Alenzi
//
//  This script reads the GPS data stream from the Arduino Serial monitor and parses it for the necessary data.
//
//  For function description overview please see associated header file.

#include "gps.h"

char gps_buffer[82];
int gps_i = 0;
char gps_time[10] = {0,0,0,0,0,0,0,0,0,0};
char temp_lat[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
char N_S[2] = {0,0};
char temp_long[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
char E_W[2] = {0,0};
char fix;
char sats[3] = {0,0,0};
char gps_alt[8] = {0,0,0,0,0,0,0,0};


void setupGPS(void) {
    Serial.begin(115200);
    Serial1.begin(9600);    // Start serial reading on pins 19 (RX) and 18 (TX)
}

void getGPSdata(void) {
    unsigned char c;
    while (Serial1.available() > 0) {
        c = Serial1.read();
        
        if ((c == '$') || (gps_i >= 80)) {
            gps_i = 0;
            //Serial.println("Reading Line!");
        }
        
        if (c != '\r') {
            gps_buffer[gps_i++] = c;
        }
        
        if (c == '\n') {
            readNMEA();
            //Serial.println("Reading NMEA!");
            Serial.print(gps_buffer);
            Serial.println();
            gps_i = 0;
        }
    }
}

void readNMEA(void) {
    /* Reads GGA NMEA sentence */
    if ((gps_buffer[3] == 'G') && (gps_buffer[4] == 'G') && (gps_buffer[5] == 'A')) {
        //Serial.println("Found a GGA!");
        processGGA();
    }
}

/* Example GGA command
 $GPGGA,074900.00,4530.49589,N,12240.79693,W,2,08,1.48,64.9,M,-21.1,M,,0000*5A
 "$TAG,time: hhmmss.ss, latitude: ddmm.mmmmm, N/S indicator, longitude: ddmm.mmmmm, E/W indicator, Pos Fix indicator: 1-2-6 valid - 0 invalid, Satellites used: 0-12, dilution of precision, altitude,*,*,*,station ID, checksum\r" */
void processGGA(void) {
    int i;
    int j;
    int k;
    
    for (i = 7; (i < gps_i) && (j < 9); ++i) {
        if (gps_buffer[i] == ',') {
            ++j;
            k=0;
        } else if (j == 0) {
            gps_time[k++] = gps_buffer[i];
        } else if (j == 1) {
            temp_lat[k++] = gps_buffer[i];
        } else if (j == 2) {
            N_S[k++] = gps_buffer[i];
        } else if (j == 3) {
            temp_long[k++] = gps_buffer[i];
        } else if (j == 4) {
            E_W[k++] = gps_buffer[i];
        } else if (j == 5) {
            fix = gps_buffer[i];
        } else if (j == 6) {
            sats[k++] = gps_buffer[i];
        } else if (j == 8) {
            gps_alt[k++] = gps_buffer[i];
        }
    }
    
    
    if (fix != '0') {
        saveGPSdata();
    }
}

void saveGPSdata(void) {
    strcpy(t_data.gps_time, gps_time);
    strcpy(t_data.latitude, temp_lat);
    strcpy(t_data.NS, N_S);
    strcpy(t_data.longitude, temp_long);
    strcpy(t_data.EW, E_W);
    strcpy(t_data.num_satellites, sats);
    strcpy(t_data.gps_alt, gps_alt);
    
    Serial.println(t_data.gps_time);
    Serial.println(t_data.latitude);
    Serial.println(t_data.NS);
    Serial.println(t_data.longitude);
    Serial.println(t_data.EW);
    Serial.println(t_data.num_satellites);
    Serial.println(t_data.gps_alt);
}
