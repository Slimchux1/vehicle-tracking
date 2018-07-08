#include <SoftwareSerial.h>

SoftwareSerial gsm(7, 8);       // 8 is RX, 7 is TX of Arduino
char c;
int lat = 15;
int lng = 56;

void setup() {
    Serial.begin(9600);
    gsm.begin(9600);
    delay(1000);
    initGSM();
}

void loop() {
    Serial.println("exec url");
    send("AT+HTTPPARA=\"URL\",\"<url>&lat=" + String(lat) + "&lng=" + String(lng) + "\"");
    waitFor("OK");
    delay(2000);

    Serial.println("exec action");
    send("AT+HTTPACTION=1");
    waitFor("OK");
    delay(5000);
    lat += 2;
    lng -= 1;
}

void initGSM() {
    Serial.println("terminate http");
    send("AT+HTTPTERM");
    delay(1000);

    Serial.println("exec cpin");
    send("AT+CPIN?");
    // waitfor("OK");
    delay(1000);

    Serial.println("exec cipmode");
    send("AT+CIPMODE=0");
    waitFor("OK");
    delay(1000);

    Serial.println("exec cipmux");
    send("AT+CIPMUX=0");
    waitFor("OK");
    delay(1000);

    Serial.println("exec cgatt");
    send("AT+CGATT=1");
    waitFor("OK");
    delay(1000);

    Serial.println("exec sap31gprs");
    send("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
    waitFor("OK");
    delay(1000);

    Serial.println("exec sap31apn");
    send("AT+SAPBR=3,1,\"APN\",\"internet\"");
    waitFor("OK");
    delay(1000);

    Serial.println("exec sap11");
    send("AT+SAPBR=1,1");
    waitFor("OK");
    delay(1000);

    Serial.println("exec sap21");
    send("AT+SAPBR=2,1");
    waitFor("OK");
    delay(1000);

    Serial.println("exec httpinit");
    send("AT+HTTPINIT");
    waitFor("OK");
    delay(1000);

    Serial.println("exec cid");
    send("AT+HTTPPARA=\"CID\",1");
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


void send(String s) {
    int len = (int) s.length();
    
    if (len < 12)
        gsm.println(s);
    else {
        String subString = "";
        for (int i = 0; i < len; i = i+10) {
            if (i + 10 < len)
                subString = s.substring(i, i + 10);
            else
                subString = s.substring(i);
            gsm.print(subString);
        }
        gsm.println("");
    }
}
