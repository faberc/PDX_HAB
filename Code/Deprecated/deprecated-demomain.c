//
//  main.c
//  APRS Radio
//
//  Created by Chuck Faber on 6/7/18.
//  Copyright © 2018 Portland State University. All rights reserved.
//
//  ECE 103 - Team 03: Abdulrahman Alamar, Chuck Faber, Ebtehal Alenzi
//
//  This script was the AX.25 demo script for the demonstration day. It will probably no longer work because the functions have some changes that are arduino only and will not work in typical C IDEs.

#include "main.h"

TELEM t_data = {"1533.12", "8493.01", "1630.00", "143056", "-15.03", "520.03", "1620.23", "23.45", "4"};


int main (int argc, const char * argv[]) {
    
    int ssid;
    int last;
    unsigned char ssidno;
    char ax25call[8];
    double altitude;
    char via[20];
    char address[100];
    char data[256];
    char crccheck[100];
    uint16_t crc;
    char packet[256];
    int choice = -1;


    if (strcmp(argv[1],"-d") == 0) {
        while (choice != 0) {
            printf("Entering Demo/Debug Mode\n\n");
            printf("Choose function to test:\n[1] Calculate SSID\t\t[2] Construct Callsign\n[3] Determine Path\t\t[4] Construct Address\n[5] Construct Data\t\t[6] CRC Check\n[7] Generate Packet\t\t[0] Exit\n\nEnter choice: ");
            scanf("%d", &choice);
            
            switch (choice) {
                case 0:
                    printf("Exiting Program.\n");
                    return 0;
                case 1:
                    printf("\nEnter an ID # between 0 and 15: ");
                    scanf("%d", &ssid);
                    if (ssid > 15) {
                        printf("\nNot a valid SSID\n");
                        break;
                    }
                    printf("\nIs this the last callsign? (1 or 0): ");
                    scanf("%d", &last);
                    ssidno = ssidcalc(ssid, last);
                    printf("\nSSID: %x \n", ssidno);
                    break;
                case 2:
                    callsignstr(ax25call);
                    printf("\nEdit call sign and SSID def in aprs.c\n");
                    printf("%s\n", ax25call);
                    break;
                case 3:
                    printf("\nEnter Altitude (in meters): ");
                    scanf("%lf", &altitude);
                    viastr(via, altitude);
                    printf("%s\n", via);
                    break;
                case 4:
                    printf("\nEnter Altitude (in meters): ");
                    scanf("%lf", &altitude);
                    constructAddress(address, altitude);
                    printf("\n%s\n", address);
                    break;
                case 5:
                    printf("\nThis will take data from the example structure above.\n");
                    constructData(data, t_data, "Hello, this is a test!");
                    printf("\n%s\n", data);
                    break;
                case 6:
                    printf("\nEnter a string to do a crc check on:\n");
                    fscanf(stdin, "%s", crccheck);
                    crc = crcCalc(crccheck);
                    printf("\n%x\n", crc);
                    break;
                case 7:
                    printf("\nThis will use the example structure above.\n");
                    generate_ax25packet(packet, t_data, "This is a test packet.");
                    printf("\n%s\n", packet);
                    break;
                default:
                    printf("Not a valid choice.\n");
                    break;
            }
        }
    }
    
    return 0;
}

/* Arduino Main */
//void setup() {
//
//}
//
//void loop() {
//
//}
