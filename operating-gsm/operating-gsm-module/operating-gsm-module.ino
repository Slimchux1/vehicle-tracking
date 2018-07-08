#include<SoftwareSerial.h>

//define our serial Communication
SoftwareSerial mySerial(7,8); // 9 is RX, 11 is TX

void setup() {
    Serial.begin(9600);
    // setup baud rate for GSM
    mySerial.begin(9600);
    mySerial.println("AT");
    // setup baud rate for Arduino-PC
    
    Serial.println("done!");
    delay(1000);
}

void loop() {
    if (Serial.available()) {
        switch(Serial.read()) {
            case 's':
                Serial.write("sending message will start\n");
                SendMessage("Hey! Myself coming from GSM module");
                Serial.write("sending message done!\n");
                break;
            case 'r':
                Serial.write("recieving message will start\n");
                RecieveMessage();
                Serial.write("done recieving!\n");
                break;
            case 'c':
                call();
                break;            
        }
    }

    if (mySerial.available()) {
        Serial.write(mySerial.read());
    }
}

void SendMessage(String message) {
    // set message format in text mode
    mySerial.println("AT+CMGF=1");

    delay(1000);

    // set reciever's mobile no.
    mySerial.println("AT+CMGS=\"+918758536303\"\r");

    delay(1000);

    // set the message
    mySerial.println(message);

    delay(1000);

    // ASCII code  for Ctrl+Z
    mySerial.println((char)26);

    delay(1000);
}

void  RecieveMessage() {
    // AT command to receive message
    mySerial.println("AT+CNMI=2,2,0,0,0");
    delay(1000);
}

void call() {
    Serial.println("Enter 10 digit no: ");
    
    String number = "";
    char myChar;
    
    for (int i=0; i<10; i++) {
      while (!Serial.available());

      myChar = (byte) Serial.read();

      number = number + myChar;
    }

    Serial.println("Calling on " + number);

    // AT command to call onn given number
    mySerial.println("ATD" + number + ";\r\n");
}
