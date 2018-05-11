// send commands by breaking in parts
void sendToGSM(String s) {
    int len = (int)s.length();
    Serial.print(s);
    Serial.println(len);
    
    if (len < 12)
        gsm.println(s);
    else {
        String subString = "";
        for (int i = 0; i < len; i = i + 10) {
            Serial.print(i);
            if (i + 10 < len)
                subString = s.substring(i, i + 10);
            else
                subString = s.substring(i);
            Serial.print(subString);
            gsm.print(subString);
            delay(100);
        }
        Serial.println("");
        gsm.println("");
    }
}
