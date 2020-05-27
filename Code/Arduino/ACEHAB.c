//
//  ACEHAB.c
//  APRS Radio
//
//  Created by Chuck Faber on 6/11/18.
//  Copyright © 2018 Portland State University. All rights reserved.
//
//  ECE 103 - Team 03: Abdulrahman Alamar, Chuck Faber, Ebtehal Alenzi
//
//  This script is the main Arduino void setup and loop script which calls the other functions.
//
//  For function description overview please see associated header file.

extern "C"{
#include "ACEHAB.h"
};

/* Transmission Timing */
unsigned long long int tx_timer = millis()/delay_correct;
long int next_packet;
int rand_delay;

/* Packet Formation */
char s[256];

void setup() {
    // put your setup code here, to run once:
    waveGenerate(); // Complete Wave Generation setup.
    setupGPS();     // Initializes the GPS
    init_sensors(); // Initializes the I2C sensors
}

void loop() {
    // put your main code here, to run repeatedly:
    
    //FIXME: Add in code to call GPS and Sensor functions to fill data structure here.
    
    if ( (millis()/delay_correct >= tx_timer) && is_bufferempty() && atoi(t_data.num_satellites) >= 4) {
        read_temp();
        read_press_alt();
        getGPSdata();       // Gets GPS data and saves it to the telemetry structure
        
        generate_ax25packet(s, t_data, "ECE103 - Hi Dr. Wong!");   // Generates the packets using the scripts in aprs.c
        buffer_ax25packet(s);   // Adds packets to transmit buffer so ISRs can send them out.
        
        /* Set time for next transmission */
        rand_delay = random(-10, 11);
        next_packet = 60 * 1000 + rand_delay * 1000;
    } else {
        next_packet = 0;
    }
    tx_timer = millis()/delay_correct + next_packet;
}
