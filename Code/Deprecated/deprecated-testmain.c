//
//  main.c
//  BitWiseStuff
//
//  Created by Chuck Faber on 5/25/18.
//  Copyright © 2018 Portland State University. All rights reserved.
//
//  ECE 103 - Team 03: Abdulrahman Alamar, Chuck Faber, Ebtehal Alenzi
//
//  This is just a test script used in development.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#define CALLSIGN "K5TRL"
#define CALLSSID 11



unsigned char ssidcalc ( unsigned char ssid, _Bool last) {
    unsigned char ssidByte;
    unsigned char notLastID = 0x60; // bitmask for 011 0000 0
    unsigned char lastID = 0x61; // bitmask for 011 0000 1
    ssid = ssid << 1; // shift SSID over one bit
    
    if (last) {
        ssidByte = ssid | lastID; // applies the last call sign bitmask
    } else {
        ssidByte = ssid | notLastID; // applies the other bitmask
    }
    
    return ssidByte;
}

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


void viastr ( double altitude, char *via) {
    if (altitude > 1500) {
        sprintf(via, "WIDE2 %c", ssidcalc(1, 1)); // Sets the path to WIDE2-1 (last callsign)
    } else {
        sprintf(via, "WIDE1 %cWIDE2 %c", ssidcalc(1, 0), ssidcalc(1, 1)); // Sets the path to WIDE1-1,WIDE2-1 (last callsign)
    }
}

uint16_t crcCalc (const char *s) {
    uint16_t crc = 0xffff;
    while ( *s != 0) {
        crc = crc ^ (*s << 8);
        for (int i = 0; i < 8; ++i) {
            if (crc & 0x8000){
                crc = (crc << 1) ^ 0x1021;
            } else {
                crc = crc << 1;
            }
            crc = crc & 0xffff;
        }
        ++s;
    }
    return crc;
}

void constructData (char* datastr, const char* time, const char* lat, const char* longitude, const char* gps_alt, const char* num_sats /*FIXME data type*/, const char* temp, const char* press, const char* alt, const char* humid, const char* message)
{
    memset(datastr, 0, 256);                     // Clear datastr
    sprintf(datastr, "%s,@%sN/%sE!%s,%s,t%s,p%s,a%s,h%s,#%s#", time, lat, longitude, gps_alt, num_sats, temp, press, alt, humid, message);
    return;
}


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


/* Reverse Bytes Nonsense */
//uint8_t reversebyte(const unsigned char byte) {
//    uint8_t rev = 0x00;
//    for ( int i = 0; i < 7; i++) {
//        rev = rev | ((byte >> i) & 0x01);
//        rev = rev << 1;
//    }
//    return rev;
//}
//
//uint16_t revcrc(uint16_t crc) {
//    uint16_t rev = 0;
//    for (int i=0; i<16; i++) {
//        rev = rev | (((crc>>i) & 0x01) << (15-i));
//    }
//    return rev;
//}
//
//unsigned short int crcCalc(const unsigned char *s/*, unsigned char crc*/) {
//    uint16_t crc = 0xffff; // crc should be initialized to 1111 1111
//
//    while (*s != 0) {   // while not the null character in the string
//        crc = crc ^ reversebyte(*s);     // XOR with the input byte
//
//        // Calculates CRC for each bit in the byte
//        for (int i = 0; i < 8; i++) {
//            if ( crc & 0x0001) {            // if the LSB is 1
//            //if ( (crc & 0x8000) >> 15 ) {            // if the LSB is 1
//
//                crc = (crc >> 1) ^ 0x8408;  // shift the CRC over by 1 and XOR it with 0x8408 which is the crc16-CCIT "polynomial value"
//
//                //crc = ((crc << 1) & 0xFFFF) ^ 0x1021;  // shift the CRC over by 1 and XOR it with 0x8408 which is the crc16-CCIT "polynomial value"
//
//
//            } else {                        // if the LSB is 0
//                crc = crc >> 1;             // shift it right
//                //crc = (crc << 1) & 0xFFFF;             // shift it right
//
//            }
//
//        }
//        ++s; // increment to point to next character in the string.
//    }
//
//    return crc;
//}


int main(int argc, const char * argv[]) {
    
    unsigned long int strlength;
    int altitude = 300;
    char via[20];
    char data[256] = "Testing Beacon";
    char ax25callsign[8];
    
    callsignstr(ax25callsign);
    
    viastr(altitude, via);
    printf("%s\n", via);
    
    /* callsign + ssid (7), aprs + ssid (7), via + ssid (7 or 14), control (1), pid (1), message length (variable), fcsh (1), fcsl(1). Non-variable length: 32 or 25 */
    //constructData(data, "123456", "1234.30", "09876.34","24642","6","19.30","243.14","21340.20","35","Hello");
    strlength = 7 + 7 + strlen(via) + 2 + strlen(data);
    //char s[strlength+1];
    char s[256];
    memset(s, 0, strlength+1);
    strcat(s, ax25callsign);
    strcat(s, via);
    
    char ctrlPID[3] = {0x3F, 0xf0, 0x00};   // Control ID and Protocol ID
    printf("\n%x %x\n", ctrlPID[0], ctrlPID[1]);
    strcat(s, ctrlPID);
    
    strcat(s, data);
    
    uint16_t crc = crcCalc(s);
//    printf("0x%x\n", crc);
    
    uint8_t crclo;
    uint8_t crchi;
    
    crclo = crc ^ 0xff;
    crchi = (crc >> 8) ^ 0xff;
    
    char crcstr[3] = {crclo, crchi, '\0'};
    printf("%x\n", crclo);
    printf("%x\n", crchi);
    strcat(s, crcstr);
    printf("%s\n", s);

    
    
    return 0;
}

