//
//  packetreader.c
//  APRS Radio
//
//  Created by Chuck Faber on 6/3/18.
//  Copyright © 2018 Portland State University. All rights reserved.
//
//  ECE 103 - Team 03: Abdulrahman Alamar, Chuck Faber, Ebtehal Alenzi
//
//  This script connects to the APRS server through a socket connection, logs into the server, and writes raw packet data to RawPackets.txt.
//
//  For function description overview please see associated header file.

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>      // Defines data types for socket.h
#include <sys/socket.h>     // Sockets Library and Functions
#include <netinet/in.h>     // Uses to parse internet domain addresses
#include <netdb.h>          // Defines hostent structure
#include <string.h>         // Basic string functions

#define SERVERADDRESS "rotate.aprs.net"
#define PORT 14580


void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    char filter[7];
    char login[256];
    printf("Enter packet filter: ");
    fscanf(stdin, "%[^\n]", filter);
    sprintf(login, "user K5TRL pass 8325 vers ece103group3 1.0 filter b/%s*\n", filter);
    
    char aprsServer[20] = SERVERADDRESS;    //Defines host address
    
    int sockfd, portno, n;  // sockfd is the file descriptor, portno is the port to connect to, and n is variable to write File I/O status to
    
    struct sockaddr_in serv_addr;
    struct hostent *server;
    
    char buffer[256];       // String buffer to read and write to.
    
    portno = PORT;          // Define port number as an integer
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    FILE* sockfp = fdopen(sockfd, "a+"); // Use the file descriptor from socket() to open a file stream in append mode for File I/O
    
    if (sockfd < 0) {
        printf("ERROR opening socket");     // Socket connection didn't work.
        exit(0);
    }
    
    server = gethostbyname(aprsServer);
    if (server == NULL) {
        printf("ERROR, no such host\n");    // Host name not in proper format.
        exit(0);
    }
    
    memset((char *) &serv_addr, 0, sizeof(serv_addr));      //Initialize the ser_addr structure to all 0s.
    serv_addr.sin_family = AF_INET;
    strncpy((char *)&serv_addr.sin_addr.s_addr,(char *)server->h_addr,server->h_length); // Copy host address into serv_addre structure.
    serv_addr.sin_port = htons(portno);     // Copy port number into serv_addr structure as a network byte.
    
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)        // Couldn't connect.
        error("ERROR connecting");
    
    
    memset(buffer, 0, 256);         // Clear the string buffer
    rewind(sockfp);                 // Go to the beginning of the socket stream
    if (fgets(buffer, 255, sockfp) == NULL)     // Store the server identification string into the buffer.
        error("ERROR reading from socket");
    printf("%s\n",buffer);                      // Print initial server identification
    
    //char login[256] = "user K5TRL pass 8325 vers ece103group3 1.0 filter b/K5TRL*\n";        // Login string -- unique to Chuck Faber
    /* Login format is user CALLSIGN pass PASSCODE ver PROGRAM-NAME VERSIONNO filter FILTER SETTINGS\n */
    n = fputs(login, sockfp);       // Write login string to the socket.
    if (n == EOF)
        error("ERROR writing to socket");
    
    memset(buffer, 0, 256);
    if(fgets(buffer, 255, sockfp) == NULL)      // Write the login status string to the buffer and display it
        error("ERROR reading from socket");
    printf("%s\n",buffer);
    memset(buffer, 0, 256);
    
    
    FILE* fp = fopen("RawPackets.txt", "a+");       // Open RawPackets file in append mode
    if (fp == NULL) {
        printf("ERROR opening file to record packets.\n");
    }
    
    for (int i = 0; i < 5; ++i) {                       // When we start sending o
        rewind(sockfp);
        if (fgets(buffer, 255, sockfp) != NULL) {       // Write packet string to buffer
            if (buffer[0] != '#') {                     // If not just a server ping (they start with #)
                n = fputs(buffer, fp);                      // Write string in buffer to the RawPackets file
                if (n == EOF)
                    error("ERROR writing to file");
            }
            printf("%s\n",buffer);                      // Also display on console
            memset(buffer, 0, 256);                     // Clear buffer
        }
    }
    fclose(fp);
    
    return 0;
}
