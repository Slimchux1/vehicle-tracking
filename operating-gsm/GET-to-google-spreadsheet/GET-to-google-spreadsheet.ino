#include<SoftwareSerial.h>

String command = "";
float lat = 15.41;
float lng = 54.75;
// define serial Communication for Arduino-GSM
SoftwareSerial gps(9, 11); // 9 is RX, 11 is TX of Arduino


void setup() {
    // set baud rate for GSM-Arduino
    gps.begin(9600);
    // set baud rate for Arduino-PC
    Serial.begin(9600);

    delay(1000);
}


void loop() {
    if (Serial.available()) {
        // get command string
        command = getString();
        //Serial.println("Command received:" + command);

        // give command to connected module
        if (command != "z")    
            gps.println(command);
        else {
            Serial.println("Entered CTRL+Z");
            enterCTRLZ();
        }    
        command = "";
    }

    // if available, print response on serial monitor
    if (gps.available()) {
        Serial.write(gps.read());
    }
}


// get string from serial monitorm
String getString() {
    String string = "";
    char c = Serial.read();
    string += c;
    delay(10);

    while (Serial.available()) {
        c = Serial.read();
        string += c;
        delay(10);
    }
    
    if (string == "z")
        return "z";
        
    else if (string == "cpin")
        return "AT+CPIN?";
        
    else if (string == "csq")
        return "AT+CSQ";
        
    else if (string == "cipmode")
        return "AT+CIPMODE?";
        
    else if (string == "cipmux")
        return "AT+CIPMUX?";
        
    else if (string == "creg")
        return "AT+CREG?";
        
    else if (string == "cgatt")
        return "AT+CGATT?";
        
    else if (string == "cstt")
        return "AT+CSTT=\"airtelgprs.com\",\"\",\"\"";
        
    else if (string == "ciicr")
        return "AT+CIICR";
        
    else if (string == "cifsr")
        return "AT+CIFSR";
        
    else if (string == "cipsend")
        return "AT+CIPSEND";
    
    else if (string == "sap31gprs")
        return "AT+SAPBR=3,1,\"Contype\",\"GPRS\"";
    
    else if (string == "sap31apn")
        return "AT+SAPBR=3,1,\"APN\",\"airtelgprs.com\"";
    
    else if (string == "sap11")
        return "AT+SAPBR=1,1";
        
    else if (string == "sap21")
        return "AT+SAPBR=2,1";
        
    else if (string == "init")
        return "AT+HTTPINIT";
        
    else if (string == "cid")
        return "AT+HTTPPARA=\"CID\",1";
        
    else if (string == "url") {
        lat = lat + 1.0;
        lng = lng + 1.0;
        return "AT+HTTPPARA=\"URL\",\"<url you want to request>\"";
    }        
 
    else if (string == "term")
        return "AT+HTTPTERM";
    
    else if (string == "action")
        return "AT+HTTPACTION=1";

    else if (string == "read")
        return "AT+HTTPREAD";
        
    else
        return string;
}

//lat=" + String(lat) + "&lng=" + String(lng)

void enterCTRLZ() {
    gps.println((char)26);
}
