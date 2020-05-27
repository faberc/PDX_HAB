#include<Wire.h>

// MPL3115A2 I2C address is 0x60(96)
#define Addr 0x60

void setup()
{
  // Initialise I2C communication
  Wire.begin();
  // Initialise Serial Communication, set baud rate = 9600
  Serial.begin(9600);
  // Start I2C transmission
  Wire.beginTransmission(Addr);
  // Select control register
  Wire.write(0x26);
  // Active mode, OSR = 128, altimeter mode
  Wire.write(0xB9);
  // Stop I2C transmission
  Wire.endTransmission();
  // Start I2C transmission
  Wire.beginTransmission(Addr);
  // Select data configuration register
  Wire.write(0x13);
  // Data ready event enabled for altitude, pressure, temperature
  Wire.write(0x07);
  // Stop I2C transmission
  Wire.endTransmission();
  delay(300);

}

void loop()
{
  unsigned int data[6];

  // Start I2C transmission
  Wire.beginTransmission(Addr);
  Wire.write(0x26);
  Wire.write(0xB9);
 // Stop I2C transmission
  Wire.endTransmission();
  delay(1000);
  // Start I2C transmission
  Wire.beginTransmission(Addr);
  // Select data register
  Wire.write(0x00);
  // Stop I2C transmission
  Wire.endTransmission();
  // this requests 6 bytes of data
  Wire.requestFrom(Addr, 6);
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
  int Temperature = ((data[4] * 256) + (data[5] & 0xF0)) / 16;
  float Altitude = tHeight / 16.0;
  float Temp_in_celsius= (Temperature / 16.0);
  float Temp_in_fahrenheit= Temp_in_celsius * 1.8 + 32;

  // this Start I2C transmission
  Wire.beginTransmission(Addr);
  // Select control register
  Wire.write(0x26);
  // Active mode, OSR = 128, barometer mode
  Wire.write(0x39);
  // Stop I2C transmission
  Wire.endTransmission();
  delay(1000);
  // Start I2C transmission
  Wire.beginTransmission(Addr);
  // Select data register
  Wire.write(0x00);
  // Stop I2C transmission
  Wire.endTransmission();
  // Request 4 bytes of data
  Wire.requestFrom(Addr, 4);
  
  // Read 4 bytes of data
  if (Wire.available() == 4)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
    data[2] = Wire.read();
    data[3] = Wire.read();
  }
  
  // this converts the data to 20-bits
  long pres = (((long)data[1] * (long)65536) + (data[2] * 256) + (data[3] & 0xF0)) / 16;
  float Pressure = (pres / 4.0) / 1000.0;

  // Output data to the serial monitor
  Serial.print("Altitude : ");
  Serial.print(Altitude);
  Serial.println(" m");
  Serial.print("Pressure : ");
  Serial.print(Pressure);
  Serial.println(" kPa");
  Serial.print("Temperature in Celsius : ");
  Serial.print(Temp_in_celsius);
  Serial.println(" C");
  Serial.print("Temperature in Fahrenheit : ");
  Serial.print(Temp_in_fahrenheit);
  Serial.println(" F");

  
  delay(500);
}
