#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <conio.h>

#define BLACK 0
#define BLUE FOREGROUND_BLUE
#define RED FOREGROUND_RED
#define GREEN FOREGROUND_GREEN
#define YELLOW FOREGROUND_RED | FOREGROUND_GREEN
#define MAGENTA FOREGROUND_BLUE | FOREGROUND_RED
#define WHITE FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED
//
typedef struct packets{
char *call_sign;
char *path1;
char *path2;
char *relayed_by;
char *time;
char *latitude;
char *longtitude;
char *GPS_altitude;
char *number_of_sattelites;
char *temperature;
char *pressure;
char *altitude;
char *humidity;
char *message;
char *dummy;

}packets;

void textcolor (int color)
{SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color); }

void intro(void)
{

int j=0;

textcolor(WHITE);
printf("                     ##     ##    ###    ########        ##     #####    #######\n");
Sleep(500);
printf("                     ##     ##   ## ##   ##     ##     ####    ##   ##  ##     ##\n");
Sleep(500);
printf("                     ##     ##  ##   ##  ##     ##       ##   ##     ##        ##\n");
Sleep(500);
//textcolor(BLUE);
printf("                     ######### ##     ## ########        ##   ##     ##  #######\n");
Sleep(500);
printf("                     ##     ## ######### ##     ##       ##   ##     ##        ##\n");
Sleep(500);
printf("                     ##     ## ##     ## ##     ##       ##    ##   ##  ##     ##\n");
Sleep(500);
//textcolor(MAGENTA);
printf("                     ##     ## ##     ## ########      ######   #####    #######\n\n");
Sleep(500);
textcolor(WHITE);
system("cls");

for(j=0; j<5; j++)
    {
        textcolor(RED);
        printf("                     ##     ##    ###    ########        ##     #####    #######\n");
        printf("                     ##     ##   ## ##   ##     ##     ####    ##   ##  ##     ##\n");
        printf("                     ##     ##  ##   ##  ##     ##       ##   ##     ##        ##\n");
        textcolor(BLUE);
        printf("                     ######### ##     ## ########        ##   ##     ##  #######\n");
        printf("                     ##     ## ######### ##     ##       ##   ##     ##        ##\n");
        printf("                     ##     ## ##     ## ##     ##       ##    ##   ##  ##     ##\n");
        textcolor(MAGENTA);
        printf("                     ##     ## ##     ## ########      ######   #####    #######\n\n");
        textcolor(WHITE);

        if(j<4)
        {
        Sleep(500);
        system("cls");
        Sleep(600);
        }
    }
}


int main () {

int j;
char Packet_info[6][200];
packets Packets[5];
packets what;


//this is the visual colorful introduction
intro();



//make this file opening thing modular

FILE *Packet_File = NULL;
Packet_File = fopen("SampleRawPackets.txt","r");

if(Packet_File == NULL)
    {
        printf("couldn't open file_elements.txt\n");
        exit(-1);
    }


 j=0;
while(!feof(Packet_File))
    {
            fgets(Packet_info[j],200,Packet_File);
            printf("%s\n",Packet_info[j]);
            j = j+1;
    }

what.call_sign= strtok(Packet_info[0],",");
what.path1 = strtok(NULL,",");
what.path2 = strtok(NULL,",");
what.dummy = strtok(NULL,",");
//RelayedByAndTime = strtok(NULL,",");
what.relayed_by = strtok(NULL,",");
what.time = strtok(NULL,",");
what.latitude = strtok(NULL,",");
what.longtitude = strtok(NULL,",");
what.GPS_altitude = strtok(NULL,",");
what.number_of_sattelites = strtok(NULL,",");
what.temperature = strtok(NULL,",");
what.pressure = strtok(NULL,",");
what.altitude = strtok(NULL,",");
what.humidity = strtok(NULL,",");
what.message = strtok(NULL,",");


textcolor(GREEN);
printf("Callsign: ");
textcolor(WHITE);
printf("%s\n",what.call_sign);

textcolor(GREEN);
printf("Path: ");
textcolor(WHITE);
printf("%s , %s\n",what.path1,what.path2);

textcolor(GREEN);
printf("Relayed by: ");
textcolor(WHITE);
printf("%s\n",what.relayed_by);

textcolor(GREEN);
printf("Time: ");
textcolor(WHITE);
printf("%s\n",what.time);

textcolor(GREEN);
printf("Latitude: ");
textcolor(WHITE);
printf("%s\n",what.latitude);

textcolor(GREEN);
printf("Longtitude: ");
textcolor(WHITE);
printf("%s\n",what.longtitude);

textcolor(GREEN);
printf("Altitude: ");
textcolor(WHITE);
printf("%s\n",what.altitude);

textcolor(GREEN);
printf("No. of connected sattellites: ");
textcolor(WHITE);
printf("%s\n",what.number_of_sattelites);

textcolor(GREEN);
printf("Temperature: ");
textcolor(WHITE);
printf("%s\n",what.temperature);

textcolor(GREEN);
printf("Pressure: ");
textcolor(WHITE);
printf("%s\n",what.latitude);

textcolor(GREEN);
printf("Altitude: ");
textcolor(WHITE);
printf("%s\n",what.altitude);

textcolor(GREEN);
printf("Humidity: ");
textcolor(WHITE);
printf("%s\n",what.humidity);

textcolor(GREEN);
printf("Message: ");
textcolor(WHITE);
printf("%s\n\n",what.message);

printf("         ,__                                                  _,");
Sleep(500);
printf("\n");
printf(" \\~\\|  ~~---___              ,                          | \\""");
Sleep(500);
printf("\n");
printf("  | Wash./ |   ~~~~~~~|~~~~~| ~~---,                VT_/,ME>");
Sleep(500);
printf("\n");
printf(" /~-_--__| |  Montana |N Dak\\ Minn/ ~\\~~/Mich.     /~| ||,'");
Sleep(500);
printf("\n");
printf(" |Oregon /  \\         |------|   { WI / /~)     __-NY',|_\\,NH");
Sleep(500);
printf("\n");
printf("/   ");
Sleep(500);
textcolor(RED);
printf("*");
textcolor(WHITE);
printf("   |Ida.|~~~~~~~~|S Dak.\\    \\   | | '~\\  |_____,|~,-'Mass.");
Sleep(500);
printf("\n");
printf("|~~--__ |    | Wyoming|____  |~~~~~|--| |__ /_-'Penn.{,~Conn (RI)");
Sleep(500);
printf("\n");
printf("|   |  ~~~|~~|        |    ~~\\ Iowa/  `-' |`~ |~_____{/NJ");
Sleep(500);
printf("\n");
printf("|   |     |  '---------, Nebr.\\----| IL|IN|OH,' ~/~\\,|`MD (DE)");
Sleep(500);
printf("\n");
printf("',  \\ Nev.|Utah| Colo. |~~~~~~~|    \\  | ,'~~\\WV/ VA |");
Sleep(500);
printf("\n");
printf("|Cal\    |    |       | Kansas| MO  \\_-~ KY /`~___--\\""");
Sleep(500);
printf("\n");
printf("',   \\  ,-----|-------+-------'_____/__----~~/N Car./");
Sleep(500);
printf("\n");
printf(" '_   '\\|     |      |~~~|Okla.|    | Tenn._/-,~~-,/");
Sleep(500);
printf("\n");
printf("    \\    |Ariz.| New  |   |_    |Ark./~~|~~\    \,/S Car.");
Sleep(500);
printf("\n");
printf("      ~~~-'     | Mex. |     `~~~\\___|MS |AL | GA /");
Sleep(500);
printf("\n");
printf("          '-,_  | _____|          |  /   | ,-'---~\\""");
Sleep(500);
printf("\n");
printf("             `~'~   \\    Texas    |LA`--,~~~~-~~,FL\\""");
Sleep(500);
printf("\n");
printf("                     \\/~\\      /~~~`---`         |  \\""");
Sleep(500);
printf("\n");
printf("                         \\    /                   \\  |");
Sleep(500);
printf("\n");
printf("                          \\  |                     '\\'");
Sleep(500);
printf("\n");
printf("                           `~'");

textcolor(BLACK);
  return 0;
}
