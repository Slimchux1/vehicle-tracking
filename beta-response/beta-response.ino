#include <SoftwareSerial.h>

SoftwareSerial gsm(8, 9);       // 8 is RX, 9 is TX of Arduino
char c;
int lat = 15;
int lng = 17;

void setup() {
    Serial.begin(9600);
    gsm.begin(9600);
    delay(1000);
}

void loop() {
    Serial.println("exec cpin");
    gsm.println("AT+CPIN?");
    waitFor("OK");
    delay(1000);

    Serial.println("exec cipmode");
    gsm.println("AT+CIPMODE?");
    waitFor("0");
    delay(1000);

    Serial.println("exec cipmux");
    gsm.println("AT+CIPMUX?");
    waitFor("0");
    delay(1000);

    Serial.println("exec cgatt");
    gsm.println("AT+CGATT=1");
    waitFor("OK");
    delay(1000);

    Serial.println("exec sap31gprs");
    gsm.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
    waitFor("OK");
    delay(1000);

    Serial.println("exec sap31apn");
    gsm.println("AT+SAPBR=3,1,\"APN\",\"airtelgprs.com\"");
    waitFor("OK");
    delay(1000);

    Serial.println("exec sap11");
    gsm.println("AT+SAPBR=1,1");
    waitFor("ERROR");
    delay(1000);

    Serial.println("exec sap21");
    gsm.println("AT+SAPBR=2,1");
    waitFor("OK");
    delay(1000);

    Serial.println("exec httpterm");
    gsm.println("AT+HTTPTERM");
    waitFor("OK");
    delay(1000);

    Serial.println("exec httpinit");
    gsm.println("AT+HTTPINIT");
    waitFor("OK");
    delay(1000);

    Serial.println("exec cid");
    gsm.println("AT+HTTPPARA=\"CID\",1");
    waitFor("OK");
    delay(1000);

    Serial.println("exec url");
    gsm.println("AT+HTTPPARA=\"URL\",\"http://api.pushingbox.com/pushingbox?devid=<ID>&lat=" + String(lat) + "&lng=" + String(lng) + "\"");
    waitFor("OK");
    delay(1000);

    Serial.println("exec action");
    gsm.println("AT+HTTPACTION=1");
    waitFor("OK");
    delay(1000);
}

void waitFor(String response) {   
    int index = 0;
    int len = (int) response.length();
    String res = "";

    while (!res.equals(response)) {
        while (index < len && gsm.available()) {
            c = (char) gsm.read();
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

