#include<SoftwareSerial.h>

String command = "";
// define serial Communication for Arduino-GSM
SoftwareSerial mySerial(8, 9); // 9 is RX, 11 is TX of Arduino


void setup() {
    // set baud rate for GSM-Arduino
    mySerial.begin(9600);
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
            mySerial.println(command);
        else
            Serial.println("Entered CTRL+Z");
        command = "";
    }

    // if available, print response on serial monitor
    if (mySerial.available()) {
        Serial.write(mySerial.read());
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
    if (string == "z") {
        enterCTRLZ();
        return "z";
    }
    else
        return string;
}

void enterCTRLZ() {
    mySerial.println((char)26);
}
