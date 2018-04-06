#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>
File dataFile;

SoftwareSerial GPSModule(2, 3); // 2 is RX, 3 is TX of Arduino
SoftwareSerial gsm(8, 9);    // 8 is RX, 9 is TX of Arduino

// process and flow related variables
short int updates;
short int pos;
short int stringplace = 0;

// update data freq on google sheet
const unsigned short int interval = 30000; // in seconds
unsigned int currentMillis, prevMillis = millis();
String lastLat = "null";
String lastLng = "null";

const short int SIZE = 12;
//String nmea[12];
//String labels[12] {"Time: ", "Lat: ", "N/S: ", "Lng: ", "E/W: ", "Fix: ", "Sats: ", "HDP: ", "Alt: ", "Metres: ", "MSL: ", "Metres: "};

String nmea[SIZE];
// String labels[SIZE] = {"Time: ", "Lat: ", "N/S: ", "Lng: ", "E/W: ", "Fix: "};

void setup()
{
	Serial.begin(9600);
  GPSModule.begin(9600);
  gsm.begin(9600);
  
  delay(3000);

  pinMode(4, OUTPUT);

  if (!SD.begin()) {
    // don't do anything more 
    Serial.println("failed");
  } 
  else {
    Serial.println("ok");
  }

  initGSM();
}



void loop() {
  dataFile = SD.open("gps.csv", FILE_WRITE);
  
  // flush make sure every command waiting in buffer executes
  // it is kind of delay until all commands in buffer executes
  Serial.flush();
  GPSModule.flush();
  gsm.flush();

  while (GPSModule.available() > 0) {
    GPSModule.read();
  }

  if (GPSModule.find("$GPGGA,")) {
    String tempMsg = GPSModule.readStringUntil('\n');
  
    //Serial.println(tempMsg);
    for (int i = 0; i < tempMsg.length(); i++) {
      if (tempMsg.substring(i, i + 1) == "," && pos < SIZE) {
      nmea[pos] = tempMsg.substring(stringplace, i);
      stringplace = i + 1;
      pos++;
      }
      if (i == tempMsg.length() - 1) {
        if (pos < SIZE)
        nmea[pos] = tempMsg.substring(stringplace, i);
      }
    }

      updates++;

    
  
      // Test to see if GPS is fixed
    if(nmea[5].toInt() > 0){
      nmea[1] = ConvertLat();
      nmea[3] = ConvertLng();

      // save data to SD if data is fixed
      if (dataFile)
      {
        Serial.println("writing");

        // update number
        dataFile.print(updates);
        dataFile.print(",");

        // time
        dataFile.print(nmea[0]);
        dataFile.print(",");

        // latitude
        dataFile.print(nmea[1]);
        dataFile.print(",");
        lastLat = nmea[1];

        // longitude
        dataFile.print(nmea[3]);
        dataFile.println("");
        lastLng = nmea[3];

        Serial.println("done");
    }
    else {
      Serial.println("error");  
    }  
    }
  } 
  else {
    Serial.println("gps not available");
  }

  
  // Print out data to the Serial Monitor
  for (int i = 0; i < 11; i++) {
    //Serial.print(labels[i]);
    Serial.print(nmea[i]);
    Serial.print(" ");
  }
  
  Serial.println(" ");

  // after certain interval update data on google sheet if correct data available  
  currentMillis = millis();
  if (currentMillis - prevMillis >= interval) {
    updateDataOnSheet(lastLat, lastLng);
    prevMillis = millis();
  }

  // Zero our flow control varibles
  stringplace = 0;
  pos = 0;
  dataFile.close();
}

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

void initGSM() {
  Serial.println("initialising gsm");

  Serial.println("exec cipmode");
  // set non-transparent mode
  gsm.println("AT+CIPMODE=0");
  waitFor("OK");
  delay(5000);

  Serial.println("exec cipmux");
  // set single connection mode
  gsm.println("AT+CIPMUX=0");
  waitFor("OK");
  delay(5000);

  Serial.println("exec cgatt");
  // enable GPRS
  gsm.println("AT+CGATT=1");
  waitFor("OK");
  delay(5000);

  Serial.println("exec sap31gprs");
  // set connection type to GPRS
  gsm.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
  waitFor("OK");
  delay(5000);

  Serial.println("exec sap31apn");
  // set APN according to your network provider
  gsm.println("AT+SAPBR=3,1,\"APN\",\"airtelgprs.com\"");
  waitFor("OK");
  delay(5000);

  Serial.println("exec sap11");
  gsm.println("AT+SAPBR=1,1");
  waitFor("ERROR");
  delay(5000);

  Serial.println("exec sap21");
  gsm.println("AT+SAPBR=2,1");
  waitFor("OK");
  delay(5000);

  Serial.println("exec httpterm");
  // terminate existing connection
  gsm.println("AT+HTTPTERM");
  waitFor("OK");
  delay(5000);

  Serial.println("exec httpinit");
  // initialise HTTP connection
  gsm.println("AT+HTTPINIT");
  waitFor("OK");
  delay(5000);

  Serial.println("exec cid");
  // set barrier profile
  gsm.println("AT+HTTPPARA=\"CID\",1");
  waitFor("OK");
  delay(5000);

  updateDataOnSheet("null", "null");
  Serial.println("initialisation done!");
}

void updateDataOnSheet(String lat, String lng) {
  Serial.println("exec url");
  
  // make a request to url
  gsm.print("AT+HTTPPARA=\"URL\",\"<url>\"");
  
  waitFor("OK");
  delay(5000);

  Serial.println("exec action");
  //  send request
  gsm.println("AT+HTTPACTION=1");
  waitFor("OK");
  delay(5000);
}


void waitFor(String response)
{
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

