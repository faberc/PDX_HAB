//
//  aprs.c
//  APRS Radio
//
//  Created by Chuck Faber on 5/31/18.
//  Copyright © 2018 Portland State University. All rights reserved.
//
//  ECE 103 - Team 03: Abdulrahman Alamar, Chuck Faber, Ebtehal Alenzi
//
//  This script defines the functions to generate the AX.25 packet. It creates the address and data strings and calculates the CRC check sequence.
//
//  For function description overview please see associated header file.

#include "aprs.h"

#define CALLSIGN "K5TRL"
#define CALLSSID 11
#define PATH_ALT 1500

/*
 This function takes in an SSID integer between 0 and 15 and converts it into the AX.25 format which has the following form:
 011 0000 0/1, where the first three bits are always the same, the middle four are the bits that represent the ID, and the last bit is 0
 if there are more call signs to follow and 1 if it is the last callsign.
 */
unsigned char ssidcalc ( unsigned char ssid, unsigned char is_last) {
    unsigned char ssidByte;
    unsigned char notLastID = 0x60; // bitmask for 011 0000 0
    unsigned char lastID = 0x61; // bitmask for 011 0000 1
    ssid = ssid << 1; // shift SSID over one bit
    
    if (is_last) {
        ssidByte = ssid | lastID; // applies the last call sign bitmask
    } else {
        ssidByte = ssid | notLastID; // applies the other bitmask
    }
    
    return ssidByte;
}

/*
 Formats the callsign into the AX.25 format which requires at most 7 characters for callsign and SSID. If callsign is shorter than
 6 characters, the remained characters must be filled with spaces. Call sign must be at least 3 characters long.
 */
void callsignstr (char *ax25callsign) {
    char callsign[10] = CALLSIGN;
    switch (strlen(callsign)) {
        case 6:
            sprintf(ax25callsign, "%s%c", callsign, ssidcalc(CALLSSID, 0));
            break;
        case 5:
            sprintf(ax25callsign, "%s %c", callsign, ssidcalc(CALLSSID, 0));
            break;
        case 4:
            sprintf(ax25callsign, "%s  %c", callsign, ssidcalc(CALLSSID, 0));
            break;
        case 3:
            sprintf(ax25callsign, "%s   %c",callsign, ssidcalc(CALLSSID, 0));
            break;
        default:
            printf("\nCall sign not appropriate size.\n");
            exit(0);
    };
}

/*
 This function reads the altitude, and if it is above a certain defined constant altitude in feet, it changes the path
 that the data packet will take. WIDE1-1, WIDE2-1 uses multiple hops to send data packet, whereas WIDE2-1 just uses one hop.
 The higher up the balloon goes, the less hops it will need as there are less obstructions for the signal. It produces a via
 string in the AX.25 format.
 */
void viastr (char *via, double altitude) {
    if (altitude > PATH_ALT) {
        sprintf(via, "WIDE2 %c", ssidcalc(1, 1)); // Sets the path to WIDE2-1 (last callsign)
    } else {
        sprintf(via, "WIDE1 %cWIDE2 %c", ssidcalc(1, 0), ssidcalc(1, 1)); // Sets the path to WIDE1-1,WIDE2-1 (last callsign)
    }
}


/* This is a cyclic redundancy check protocol for doing a transmission check of our data string. Based off of Wikipedia article: https://en.wikipedia.org/wiki/Computation_of_cyclic_redundancy_checks : "Code fragment 4: Shift register based division, MSB first" . The XOR operation acts like a kind of subtraction between the bits. */

unsigned short int crcCalc (const char *s) {   // Takes in the string address
    unsigned short int crc = 0xffff;    // Initializes crc to 1111 1111
    while ( *s != 0) {      // While the value at the current address is not NULL
        crc = crc ^ (*s << 8);      // Shift the character over 8 bits and xor it with the the current crc value
        for (int i = 0; i < 8; ++i) {   // For each bit in this character
            if (crc & 0x8000){      // If the most significant bit is 1
                crc = (crc << 1) ^ 0x1021;      // shift it to the left and xor it with the CRC-CCITT16 Polynomial
            } else {
                crc = crc << 1;         // else just shift it to the left
            }
            crc = crc & 0xffff;         // trim the remainder to 16 bits
        }
        ++s;        // iterate to next character in string
    }
    return crc;
}

/* If the above doesn't work the below might LSB first */
//uint16_t crcLSB (unsigned const char *s) {
//    uint16_t crc = 0xffff;
//    while ( *s != 0) {
//        crc = crc ^ *s;
//        for (int i = 0; i < 8; ++i) {
//            if (crc & 0x0001){
//                crc = (crc >> 1) ^ 0x8408;
//            } else {
//                crc = crc >> 1;
//            }
//        }
//        ++s;
//    }
//    return crc;
//}

/* This function constructs the data taken from the sensors in the correct sending format */
void constructData (char* datastr, TELEM t_data, char* message) {
    memset(datastr, 0, 256);                     // Clear datastr
    char temp_buffer[10];
    char press_buffer[10];
    char alt_buffer[10];
    char humid_buffer[10];
    dtostrf(t_data.temperature, 3, 2, temp_buffer);
    dtostrf(t_data.pressure, 3, 2, press_buffer);
    dtostrf(t_data.altitude, 3, 2, alt_buffer);
    dtostrf(t_data.humidity, 3, 2, humid_buffer);
    sprintf(datastr, "%s,@%s%s/%s%s!%s,%s,t%s,p%s,a%s,h%s,#%s#", t_data.gps_time, t_data.latitude, t_data.NS, t_data.longitude, t_data.EW, t_data.gps_alt, t_data.num_satellites, temp_buffer, press_buffer, alt_buffer, humid_buffer, message);
    return;
}

void constructAddress (char* address, double altitude) {
    char ax25callsign[8];
    char path[20];
    callsignstr(ax25callsign);
    viastr(path, altitude);
    memset(address, 0, sizeof(address)/sizeof(char));
    sprintf(address, "%s%s", ax25callsign, path);
}

void generate_ax25packet (char* s, TELEM t_data, char* message) {
    memset(s, 0, 256);
    char address[100];
    char data[256];
    float altitude = t_data.altitude;
    constructAddress(address, altitude);
    strcat(s, address);
    char ctrlPID[3] = {0x3F, 0xf0, 0x00};   // Control ID and Protocol ID
    strcat(s, ctrlPID);
    constructData(data, t_data, message);
    strcat(s, data);
    
    /* Calculate CRC for string */
    uint16_t crc = crcCalc(s);
    uint8_t crclo;
    uint8_t crchi;
    
    crclo = crc ^ 0xff;
    crchi = (crc >> 8) ^ 0xff;
    char crcstr[3] = {crclo, crchi, '\0'};
    strcat(s, crcstr);
}
