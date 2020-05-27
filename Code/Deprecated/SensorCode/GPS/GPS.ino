#include <SoftwareSerial.h>

SoftwareSerial gps(4,3);
byte gpsin;

void  setup ()
{
 Serial.begin (115200);            
 gps.begin (9600);
}


void  loop ()
{
if (gps.available()>0) // if there is data coming into the serial line
   { gpsin = gps.read(); }    
       if (gpsin == 82) //ascii code R(mc)
           {
             Serial.print("$GPR"); // as R is dropped to get next byte it is printed anyway
             while (gpsin != 42) //ascii code *, end of sentence
               {
                 if (gps.available()>0)
                   {
                     gpsin = gps.read(); // get the byte( or char) of data
                     Serial.write(gpsin); // send it to the serial monitor
                   }               
               }               
             Serial.println(" ");   
           }   
 
}
