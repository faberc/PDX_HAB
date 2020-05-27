#include <Wire.h>
// MCP9808 I2C address is 0x18(24)
#define Addr 0x18
void setup()
{  
// Initialise I2C communication as MASTER  
Wire.begin();  
// Initialise Serial Communication, set baud rate = 9600  
Serial.begin(9600);     
// Start I2C Transmission  
Wire.beginTransmission(Addr);  
// Select resolution register  
Wire.write(0x08);  
// Resolution = +0.0625 / C  
Wire.write(0x03);  
// Stop I2C Transmission  
Wire.endTransmission();
}
void loop()
{  
unsigned int data[2];    
// Starts I2C communication  
Wire.beginTransmission(Addr);  
// Select data register  
Wire.write(0x05);  
// Stop I2C transmission  
Wire.endTransmission();    
// Request 2 bytes of data  
Wire.requestFrom(Addr, 2);    
// Read 2 bytes of data  
// temp MSB, temp LSB  
if(Wire.available() == 2)  
{    
data[0] = Wire.read();    
data[1] = Wire.read();  
}    
// Convert the data to 13-bits  
int temp = ((data[0] & 0x1F) * 256 + data[1]);  
if(temp > 4095)  
{    
temp -= 8192;  
}  
float Temp_in_celsius = temp * 0.0625;
float Temp_in_Fahrenheit = Temp_in_celsius * 1.8 + 32;    
// Output data to screen  
Serial.print("Temperature in Celsius : ");  
Serial.println(Temp_in_celsius);  
Serial.println(" C");  
Serial.print("Temperature in Fahrenheit : ");  
Serial.println(Temp_in_Fahrenheit);  
Serial.println(" F");  
delay(500);
}
