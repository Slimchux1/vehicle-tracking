#include <SoftwareSerial.h>

SoftwareSerial GPSModule(2, 3); // 2 is RX, 3 is TX of Arduino
SoftwareSerial gsm(7, 8);       // 7 is RX, 8 is TX of Arduino
unsigned long int prevMillis = millis(), currentMillis;
const short int SIZE = 6;
String nmea[SIZE];
bool DEBUG = false;
/*String labels[12] {"Time: ", "Lat: ", "N/S: ", "Lng: ", "E/W: ", "Fix: ",
                     "Sats: ", "HDP: ", "Alt: ", "Metres: ", "MSL: ", "Metres: "};*/

void blinkForever();
String ConvertLat();
String ConvertLng();
void extractData();
void printToSerialMonitor();
String getResponse(short int skipLength);
void updateDataOnSheet(String lat, String lng);
void initGSM();
void sendToGSMCharByChar(String s);

#include "blinkForever.h"
#include "convertLatLng.h"
#include "extractData.h"
#include "getResponse.h"
#include "initGSM.h"
#include "printToSerialMonitor.h"
#include "updateDataOnSheet.h"
#include "sendToGSMCharByChar.h"

void setup() {
	  Serial.begin(9600);
    pinMode(4, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(9, OUTPUT);
    digitalWrite(9, LOW);
    digitalWrite(5, LOW);
        
    initGSM();

    for (short int i = 0; i < SIZE; i++)
        nmea[i] = "null";
}

void loop() {
    // flush make sure every command waiting in buffer executes
    // it is kind of delay until all commands in buffer executes
    // Serial.flush();
    // GPSModule.flush();
    // gsm.flush();
    
    GPSModule.begin(9600);
    while (GPSModule.available() > 0) {
        GPSModule.read();
    }
    
    if (GPSModule.find("$GPGGA,")) {
        if (DEBUG) Serial.println(F("GPS data found, now extracting"));
        extractData();
        if (DEBUG) Serial.println(F("GPS data extracted"));
    } else {
        if (DEBUG) Serial.println(F("gps data not available"));
    }

    if (DEBUG) printToSerialMonitor();
    
    // update data on google sheet after given interval
    currentMillis = millis();
    if (currentMillis - prevMillis >= 10000 && !nmea[1].equals("null") && !nmea[3].equals("null")) {
        updateDataOnSheet(nmea[1], nmea[3]);
        prevMillis = millis();
    }
}
