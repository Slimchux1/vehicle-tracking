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
    else if (string == "cipst")
        return "AT+CIPSTART=\"TCP\",\"script.google.com\",\"80\"";
    else if (string == "cipst2")
        return "AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"";    
    else if (string == "cipsend")
        return "AT+CIPSEND";
    else if (string == "OKOK") {
        lat = lat + 1.0;
        lng = lng + 1.0;
        return "PUT /macros/s/AKfycbysxw0m3rVjn70l7otnWSXp4MqB9kQm0zkInr1285hqHnPJprFI/exec/";
    }
    else if (string == "init")
        return "AT+HTTPINIT";
    else if (string == "cid")
        return "AT+HTTPPARA=\"CID\",1";
    else if (string == "url")
        return "AT+HTTPPARA=\"URL\",\"http://script.google.com/macros/s/\"";
    else if (string == "break1")
        return "AT+HTTPPARA=\"BREAK\",\"AKfycbysxw0m3rVjn70l7otnWSXp\"";
    else if (string == "break2")
        return "AT+HTTPPARA=\"BREAK\",\"4MqB9kQm0zkInr1285hqHnPJprFI/exec\"";
    else if (string == "data")
        return "AT+HTTPDATA=15,15000";
    else if (string == "insert")
        return "lat=17&lng=25.6";
    else
        return string;
}
//lat=" + String(lat) + "&lng=" + String(lng)
void enterCTRLZ() {
    gps.println((char)26);
}
