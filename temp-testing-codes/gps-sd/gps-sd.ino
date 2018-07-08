#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>

File dataFile;
SoftwareSerial GPSModule(2, 3); // RX, TX

short int pinCS = 4;

String nmea[6];

void setup() {
    Serial.begin(9600);
    GPSModule.begin(9600);
    pinMode(pinCS,OUTPUT);
	delay(2000);
	
	if (!SD.begin()) {
		// don't do anything more 
    	Serial.println("failed");
	} 
	else {
    	Serial.println("ok");
    }
}


void loop() {
	//flushing out data at the start of each loop 
	// waits until all commands in buffer executes
	Serial.flush();
	GPSModule.flush();

	while (GPSModule.available() > 0) {
		Serial.write(GPSModule.read());
	}
  
	if (GPSModule.find("$GPGGA,")) {
    Serial.println("GPS data extracted");
		// extract req data from GPS output
		extractData();
    Serial.println("GPS data extracted");
    
		// write to SD card if GPS is fixed
   		if(nmea[5].toInt() > 0){
			    writeToSD();
		}
	}

	printToSerialMonitor();
}


void writeToSD() {
	nmea[1] = ConvertLat();
	nmea[3] = ConvertLng();

	// Serial.println("\n<<<<<<<<<<<Lat:" + nmea[1] + "  Lng:" + nmea[3] + ">>>>>>>>>>>>>\n");

	dataFile = SD.open("gps_test.csv", FILE_WRITE);
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


// extract data from output string of GPS
void extractData() {
	short int stringStartIndex = 0, stringEndIndex = 0;

	String tempMsg = GPSModule.readStringUntil('\n');
	// Serial.println(tempMsg);
	// extract meaningful data
	for (int i = 0; i < 6; i++) {
		// find index where comma is
		while (tempMsg[stringEndIndex] != ',')
			stringEndIndex++;

		// copy substring of tempMsg from startIndex to EndIndex
		nmea[i] = tempMsg.substring(stringStartIndex, stringEndIndex);
		stringStartIndex = stringEndIndex + 1;
		stringEndIndex++;
	}
}


String ConvertLat() {
	String posneg = "";
	if (nmea[2] == "S") {
		posneg = "-";
	}
	
	String latfirst;
	float latsecond;

	for (int i = 0, len = nmea[1].length(); i < len; i++) {
		if (nmea[1][i] == '.') {
			latfirst = nmea[1].substring(0, i - 2);
			latsecond = nmea[1].substring(i - 2).toFloat();
		}
	}

	latsecond = latsecond / 60;
	String CalcLat = "";

	char charVal[9];
	dtostrf(latsecond, 4, 6, charVal);
	for (int i = 0, size = sizeof(charVal); i < size; i++) {
		CalcLat += charVal[i];
	}

	latfirst += CalcLat.substring(1);
	latfirst = posneg += latfirst;
  Serial.println(latfirst);
	return latfirst;
}


String ConvertLng() {
  	String posneg = "";
  	if (nmea[4] == "W") {
   		posneg = "-";
  	}

  	String lngfirst;
  	float lngsecond;
	for (int i = 0, len = nmea[3].length(); i < len; i++) {
		if (nmea[3][i] == '.') {
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
  	
	for (int i = 0, size = sizeof(charVal); i < size; i++) {
    	CalcLng += charVal[i];
  	}

	lngfirst += CalcLng.substring(1);
	lngfirst = posneg += lngfirst;
  Serial.println(lngfirst);
	return lngfirst;
}


// Print out data to the Serial Monitor
void printToSerialMonitor() {
	for (int i = 0; i < 6; i++) {
		Serial.print(nmea[i]);
		Serial.println("");
	}
}
