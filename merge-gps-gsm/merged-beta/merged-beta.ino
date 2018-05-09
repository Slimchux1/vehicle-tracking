#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>
File dataFile;

SoftwareSerial GPSModule(2, 3); // 2 is RX, 3 is TX of Arduino
SoftwareSerial gsm(7, 8);    // 7 is RX, 8 is TX of Arduino

// update freq on google sheet
const unsigned short int interval = 20000; // in milliseconds
unsigned int prevMillis = millis(), currentMillis;

const short int SIZE = 6;
//String labels[12] {"Time: ", "Lat: ", "N/S: ", "Lng: ", "E/W: ", "Fix: ", "Sats: ", "HDP: ", "Alt: ", "Metres: ", "MSL: ", "Metres: "};

String nmea[SIZE];
// String labels[6] = {"Time: ", "Lat: ", "N/S: ", "Lng: ", "E/W: ", "Fix: "};

void setup() {
	Serial.begin(9600);
    GPSModule.begin(9600);
    gsm.begin(9600);
    pinMode(4, OUTPUT);
    delay(2000);

    if (!SD.begin()) {
        // don't do anything more 
        Serial.println("failed");
    } 
    else {
        Serial.println("ok");
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
            writeToSD();
        }
    } else {
        // Serial.println("gps not available");
    }

    printToSerialMonitor();

    // update data on google sheet after given interval
    currentMillis = millis();
    if (currentMillis - prevMillis >= interval) {
        updateDataOnSheet(nmea[1], nmea[3]);
        prevMillis = millis();
    }
}


/********************************************************************************************/
// extract data from output string of GPS
void extractData() {
    short int stringStartIndex = 0, stringEndIndex = 0;

    String tempMsg = GPSModule.readStringUntil('\n');
    // Serial.println(tempMsg);
    // extract meaningful data
    for (int i = 0; i < SIZE; i++) {
        // find index where comma is
        while (tempMsg[stringEndIndex] != ',')
            stringEndIndex++;

        // copy substring of tempMsg from startIndex to EndIndex
        nmea[i] = tempMsg.substring(stringStartIndex, stringEndIndex);
        stringStartIndex = stringEndIndex + 1;
        stringEndIndex++;
    }
}
/********************************************************************************************/


/********************************************************************************************/
void writeToSD() {
    nmea[1] = ConvertLat();
    nmea[3] = ConvertLng();

    // Serial.println("\n<<<<<<<<<<<Lat:" + nmea[1] + "  Lng:" + nmea[3] + ">>>>>>>>>>>>>\n");

    dataFile = SD.open("MERGETEST10MAY.CSV", FILE_WRITE);
    // write if dataFile is succesfully created/opened
    if (dataFile) {
        // Serial.println("writing");

        // time
        dataFile.print(nmea[0]);
        dataFile.print(",");

        // latitude
        dataFile.print(nmea[1]);
        dataFile.print(",");

        // longitude
        dataFile.print(nmea[3]);
        dataFile.println("");

        // Serial.println("writing ends!");
    }
    else {
        // Serial.println("error");
    }

    dataFile.close();
}
/********************************************************************************************/


/********************************************************************************************/
void initGSM() {
    Serial.println("initialising gsm");

    // Serial.println("exec httpterm");
    // terminate existing connection
    sendToGSM("AT+HTTPTERM");
    delay(5000);

    // Serial.println("exec cpin");
    sendToGSM("AT+CPIN?");
    delay(5000);

    // Serial.println("exec cipmode");
    // set non-transparent mode
    sendToGSM("AT+CIPMODE=0");
    waitFor("OK");
    delay(5000);

    // Serial.println("exec cipmux");
    // set single connection mode
    sendToGSM("AT+CIPMUX=0");
    waitFor("OK");
    delay(5000);

    // Serial.println("exec cgatt");
    // enable GPRS
    sendToGSM("AT+CGATT=1");
    waitFor("OK");
    delay(10000);

    // Serial.println("exec sap31gprs");
    // set connection type to GPRS
    sendToGSM("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
    waitFor("OK");
    delay(5000);

    // Serial.println("exec sap31apn");
    // set APN according to your network provider
    sendToGSM("AT+SAPBR=3,1,\"APN\",\"internet\"");
    waitFor("OK");
    delay(7000);

    // Serial.println("exec sap11");
    sendToGSM("AT+SAPBR=1,1");
    waitFor("OK");
    delay(5000);

    // Serial.println("exec sap21");
    sendToGSM("AT+SAPBR=2,1");
    waitFor("OK");
    delay(3000);

    // Serial.println("exec httpinit");
    // initialise HTTP connection
    sendToGSM("AT+HTTPINIT");
    waitFor("OK");
    delay(3000);

    // Serial.println("exec cid");
    // set barrier profile
    sendToGSM("AT+HTTPPARA=\"CID\",1");
    waitFor("OK");
    delay(5000);

    updateDataOnSheet("init", "done");
    Serial.println("initialisation done!");
}
/********************************************************************************************/


/********************************************************************************************/
// send commands by breaking in parts
void sendToGSM(String s) {
    int len = (int)s.length();

    if (len < 12)
        gsm.println(s);
    else {
        String subString = "";
        for (int i = 0; i < len; i = i + 10) {
            if (i + 10 < len)
                subString = s.substring(i, i + 10);
            else
                subString = s.substring(i);
            gsm.print(subString);
        }
        gsm.println("");
    }
}
/********************************************************************************************/


/********************************************************************************************/
void updateDataOnSheet(String lat, String lng) {
    Serial.println("exec url");
    
    // make a request to url
    sendToGSM("AT+HTTPPARA=\"URL\",\"<url>&lat=" + lat + "&lng=" + lng + "\"");
    waitFor("OK");
    delay(5000);

    Serial.println("exec action");
    //  send request
    sendToGSM("AT+HTTPACTION=1");
    waitFor("OK");
    delay(5000);
}
/********************************************************************************************/


/********************************************************************************************/
void waitFor(String response) {
    char c;
    short int index = 0;
    short int len = (int)response.length();
    String res = "";

    while (!res.equals(response)) {
        while (index < len && gsm.available()) {
            c = (char)gsm.read();
            Serial.write(c);
            if (c == response[index]) {
                res = res + c;
                index++;
            } 
            else {
                index = 0;
                res = "";
            }
        }
    }
    
    Serial.println(" ");
}
/********************************************************************************************/


/********************************************************************************************/
// Print out data to the Serial Monitor
void printToSerialMonitor() {
    for (int i = 0; i < SIZE; i++) {
        Serial.print(nmea[i]);
        Serial.print(" ");
    }
    Serial.println("");
}
/********************************************************************************************/


/********************************************************************************************/
String ConvertLat() {
    String posneg = "";
    if (nmea[2] == "S") {
        posneg = "-";
    }

    String latfirst;
    float latsecond;

    for (int i = 0; i < nmea[1].length(); i++) {
        if (nmea[1].substring(i, i + 1) == ".") {
            latfirst = nmea[1].substring(0, i - 2);
            latsecond = nmea[1].substring(i - 2).toFloat();
        }
    }

    latsecond = latsecond / 60;
    String CalcLat = "";

    char charVal[9];
    dtostrf(latsecond, 4, 6, charVal);
    for (int i = 0; i < sizeof(charVal); i++) {
        CalcLat += charVal[i];
    }

    latfirst += CalcLat.substring(1);
    latfirst = posneg += latfirst;
    return latfirst;
}
/********************************************************************************************/


/********************************************************************************************/
String ConvertLng() {
    String posneg = "";
    if (nmea[4] == "W") {
        posneg = "-";
    }

    String lngfirst;
    float lngsecond;
    for (int i = 0; i < nmea[3].length(); i++) {
        if (nmea[3].substring(i, i + 1) == ".") {
            lngfirst = nmea[3].substring(0, i - 2);
            //Serial.println(lngfirst);
            lngsecond = nmea[3].substring(i - 2).toFloat();
            //Serial.println(lngsecond);
        }
    }

    lngsecond = lngsecond / 60;
    String CalcLng = "";
    char charVal[9];
    dtostrf(lngsecond, 4, 6, charVal);

    for (int i = 0; i < sizeof(charVal); i++) {
        CalcLng += charVal[i];
    }

    lngfirst += CalcLng.substring(1);
    lngfirst = posneg += lngfirst;
    return lngfirst;
}
