//
//  deprecated-crc.c
//  APRS Radio
//
//  Created by Chuck Faber on 6/7/18.
//  Copyright © 2018 Portland State University. All rights reserved.
//

#include <stdio.h>


// Reverse Bytes Nonsense 
uint8_t reversebyte(const unsigned char byte) {
    uint8_t rev = 0x00;
    for ( int i = 0; i < 7; i++) {
        rev = rev | ((byte >> i) & 0x01);
        rev = rev << 1;
    }
    return rev;
}

uint16_t revcrc(uint16_t crc) {
    uint16_t rev = 0;
    for (int i=0; i<16; i++) {
        rev = rev | (((crc>>i) & 0x01) << (15-i));
    }
    return rev;
}

unsigned short int crcCalc(const unsigned char *s/*, unsigned char crc*/) {
    uint16_t crc = 0xffff; // crc should be initialized to 1111 1111

    while (*s != 0) {   // while not the null character in the string
        crc = crc ^ reversebyte(*s);     // XOR with the input byte

        // Calculates CRC for each bit in the byte
        for (int i = 0; i < 8; i++) {
            if ( crc & 0x0001) {            // if the LSB is 1
            //if ( (crc & 0x8000) >> 15 ) {            // if the LSB is 1

                crc = (crc >> 1) ^ 0x8408;  // shift the CRC over by 1 and XOR it with 0x8408 which is the crc16-CCIT "polynomial value"

                //crc = ((crc << 1) & 0xFFFF) ^ 0x1021;  // shift the CRC over by 1 and XOR it with 0x8408 which is the crc16-CCIT "polynomial value"


            } else {                        // if the LSB is 0
                crc = crc >> 1;             // shift it right
                //crc = (crc << 1) & 0xFFFF;             // shift it right

            }

        }
        ++s; // increment to point to next character in the string.
    }

    return crc;
}
