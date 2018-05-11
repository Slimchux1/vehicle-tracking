#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>

SoftwareSerial GPSModule(2, 3); // 2 is RX, 3 is TX of Arduino
SoftwareSerial gsm(7, 8);    // 7 is RX, 8 is TX of Arduino
unsigned int prevMillis = millis(), currentMillis;
const short int SIZE = 6;
String nmea[SIZE];
/*String labels[12] {"Time: ", "Lat: ", "N/S: ", "Lng: ", "E/W: ", "Fix: ",
                     "Sats: ", "HDP: ", "Alt: ", "Metres: ", "MSL: ", "Metres: "};*/

void blinkForever();
String ConvertLat();
String ConvertLng();
void extractData();
String getResponse(short int skipLength);
void writeToSD();
void printToSerialMonitor();
void sendToGSM(String s);
void updateDataOnSheet(String lat, String lng);
void initGSM();


#include "blinkForever.h"
#include "convertLatLng.h"
#include "extractData.h"
#include "getResponse.h"
#include "initGSM.h"
#include "printToSerialMonitor.h"
#include "sendToGSM.h"
#include "updateDataOnSheet.h"
#include "writeToSD.h"


void setup() {
	Serial.begin(9600);
    GPSModule.begin(9600);
    gsm.begin(9600);
    pinMode(4, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    delay(2000);

    if (!SD.begin()) {
        // don't do anything more 
        Serial.println(F("failed"));
    } 
    else {
        Serial.println(F("ok"));
    }

    initGSM();

    for (int i = 0; i < SIZE; i++)
        nmea[i] = "null";
}

void loop() {
    // flush make sure every command waiting in buffer executes
    // it is kind of delay until all commands in buffer executes
    Serial.flush();
    GPSModule.flush();
    gsm.flush();

    while (GPSModule.available() > 0) {
        GPSModule.read();
    }

    if (GPSModule.find("$GPGGA,")) {
        extractData();

        // write to SD card if GPS is fixed
        if (nmea[5].toInt() > 0) {
            nmea[1] = ConvertLat();
            nmea[3] = ConvertLng();
            writeToSD();
        }
    } else {
        // Serial.println("gps not available");
    }

    // printToSerialMonitor();

    // update data on google sheet after given interval
    currentMillis = millis();
    if (currentMillis - prevMillis >= 5000) {
        updateDataOnSheet(nmea[1], nmea[3]);
        prevMillis = millis();
    }
}
