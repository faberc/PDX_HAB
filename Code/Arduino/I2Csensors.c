//
//  I2Csensors.c
//  APRS Radio
//
//  Created by Abdulrahman Alamar on 6/7/18.
//  Copyright © 2018 Portland State University. All rights reserved.
//
//
//  ECE 103 - Team 03: Abdulrahman Alamar, Chuck Faber, Ebtehal Alenzi
//
//  This script uses the I2C protocol to connect to and read from the sensors.
//
//  For function description overview please see associated header file.

#include "I2Csensors.h"
#include <Wire.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// MCP9808 I2C address is 0x18(24)
// MPL3115A2 I2C address is 0x60(96)
#define MCP9808_ADDR 0x18
#define MPL3115A2_ADDR 0x60
void init_sensors(void)
{
    
    /* Temperature Sensor Setup */
    // Initialise I2C communication as MASTER
    Wire.begin();
    // Initialise Serial Communication, set baud rate = 9600
    Serial.begin(115200);
    // Start I2C Transmission
    Wire.beginTransmission(MCP9808_ADDR);
    // Select resolution register
    Wire.write(0x08);
    // Resolution = +0.0625 / C
    Wire.write(0x03);
    // Stop I2C Transmission
    Wire.endTransmission();
    
    
    /* Pressure/Altimeter Sensor Setup */
    // Select control register
    Wire.write(0x26);
    // Active mode, OSR = 128, altimeter mode
    Wire.write(0xB9);
    // Stop I2C transmission
    Wire.endTransmission();
    // Start I2C transmission
    Wire.beginTransmission(MPL3115A2_ADDR);
    // Select data configuration register
    Wire.write(0x13);
    // Data ready event enabled for altitude, pressure, temperature
    Wire.write(0x07);
    // Stop I2C transmission
    Wire.endTransmission();
    delay(300 * delay_correct);
}

void read_temp(void) {
    
    unsigned int data[2];
    // Starts I2C communication
    Wire.beginTransmission(MCP9808_ADDR);
    // Select data register
    Wire.write(0x05);
    // Stop I2C transmission
    Wire.endTransmission();
    // Request 2 bytes of data
    Wire.requestFrom(MCP9808_ADDR, 2);
    // Read 2 bytes of data
    // temp MSB, temp LSB
    if(Wire.available() == 2)
    {
        data[0] = Wire.read();
        data[1] = Wire.read();
    }
    // Convert the data to 13-bits
    int temp_reading = ((data[0] & 0x1F) * 256 + data[1]);
    if(temp_reading > 4095)
    {
        temp_reading -= 8192;
    }
    float temp_in_celsius = temp_reading * 0.0625;
    
    t_data.temperature = temp_in_celsius;
    Serial.print("Temp in C: ");
    Serial.println(t_data.temperature);
    
    delay(500 * delay_correct);
}

void read_press_alt (void) {
    unsigned int data[6];
    
    // Start I2C transmission
    Wire.beginTransmission(MPL3115A2_ADDR);
    Wire.write(0x26);
    Wire.write(0xB9);
    // Stop I2C transmission
    Wire.endTransmission();
    delay(1000 * delay_correct);
    // Start I2C transmission
    Wire.beginTransmission(MPL3115A2_ADDR);
    // Select data register
    Wire.write(0x00);
    // Stop I2C transmission
    Wire.endTransmission();
    // this requests 6 bytes of data
    Wire.requestFrom(MPL3115A2_ADDR, 6);
    // Read 6 bytes of data from address 0x00(00)
    if (Wire.available() == 6)
    {
        data[0] = Wire.read();
        data[1] = Wire.read();
        data[2] = Wire.read();
        data[3] = Wire.read();
        data[4] = Wire.read();
        data[5] = Wire.read();
    }
    
    // this converts the data to 20-bits
    int tHeight = (((long)(data[1] * (long)65536) + (data[2] * 256) + (data[3] & 0xF0)) / 16);
    float altitude = tHeight / 16.0;
    
    // this Start I2C transmission
    Wire.beginTransmission(MPL3115A2_ADDR);
    // Select control register
    Wire.write(0x26);
    // Active mode, OSR = 128, barometer mode
    Wire.write(0x39);
    // Stop I2C transmission
    Wire.endTransmission();
    delay(1000 * delay_correct);
    // Start I2C transmission
    Wire.beginTransmission(MPL3115A2_ADDR);
    // Select data register
    Wire.write(0x00);
    // Stop I2C transmission
    Wire.endTransmission();
    // Request 4 bytes of data
    Wire.requestFrom(MPL3115A2_ADDR, 4);
    
    // Read 4 bytes of data
    if (Wire.available() == 4)
    {
        data[0] = Wire.read();
        data[1] = Wire.read();
        data[2] = Wire.read();
        data[3] = Wire.read();
    }
    
    // this converts the data to 20-bits
    long pres_reading = (((long)data[1] * (long)65536) + (data[2] * 256) + (data[3] & 0xF0)) / 16;
    float pressure = (pres_reading / 4.0) / 1000.0;
    
    t_data.altitude = altitude;
    t_data.pressure = pressure;
    
    Serial.print("Altitude (m): ");
    Serial.println(t_data.altitude);
    Serial.print("Pressure (kPa): ");
    Serial.println(t_data.pressure);
    
    delay(500 * delay_correct);
}
