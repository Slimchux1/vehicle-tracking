#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>
File dataFile;
SoftwareSerial GPSModule(2, 3); // RX, TX

// process and flow related variables
int updates;
int failedUpdates;
int pos;
int stringplace = 0;
int pinCS = 4;

String nmea[12];
String labels[12] {"Time: ", "Lat: ", "N/S: ", "Lng: ", "E/W: ", "Fix: ", "Sats: ", "HDP: ", "Alt: ", "Metres: ", "MSL: ", "Metres: "};
//


void setup() {
  Serial.begin(9600);
  GPSModule.begin(9600);
  pinMode(pinCS,OUTPUT);
   if (!SD.begin()) {
    Serial.println("failed");
    // don't do anything more 
 }
 else{
  Serial.println("ok");
 }
}

void loop() {
  dataFile = SD.open("gps.csv", FILE_WRITE);
  //flushing out data at the start of each loop, not sure it is really neccessay
  Serial.flush();
  GPSModule.flush();
  while (GPSModule.available() > 0)
  {
    GPSModule.read();

  }

  if (GPSModule.find("$GPGGA,")) {
    String tempMsg = GPSModule.readStringUntil('\n');
    //Serial.println(tempMsg);
    for (int i = 0; i < tempMsg.length(); i++) {
      if (tempMsg.substring(i, i + 1) == ",") {
        nmea[pos] = tempMsg.substring(stringplace, i);
        stringplace = i + 1;
        pos++;
      }
      if (i == tempMsg.length() - 1) {
        nmea[pos] = tempMsg.substring(stringplace, i);
      }
    }
    updates++;

    nmea[1] = ConvertLat();
    nmea[3] = ConvertLng();
    
// Test to see if GPS is fixed
   if(nmea[5].toInt() > 0){

  // save data to SD if data is fixed
  if (dataFile) {
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
    // longitude
    dataFile.print(nmea[3]);
    dataFile.println("");
    Serial.println("done");  

  }
  else{
    Serial.println("error");  
  }  
  
 }

  }
  else {

    failedUpdates++;
    Serial.println(failedUpdates);
  }

  // Print out data to the Serial Monitor
  for (int i = 0; i < 11; i++) {
     Serial.print(labels[i]);
     Serial.print(nmea[i]);
     Serial.println("");
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
  for (int i = 0; i < sizeof(charVal); i++)
  {
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
  for (int i = 0; i < sizeof(charVal); i++)
  {
    CalcLng += charVal[i];
  }
  lngfirst += CalcLng.substring(1);
  lngfirst = posneg += lngfirst;
  return lngfirst;
}



