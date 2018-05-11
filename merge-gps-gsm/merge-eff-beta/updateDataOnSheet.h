void initGSM();

void updateDataOnSheet(String lat, String lng) {
    String response = "";
    Serial.println("exec url");
    // make a request to url
    // gsm.flush();
    sendToGSM("AT+HTTPPARA=\"URL\",\"http://api.pushingbox.com/pushingbox?devid=v15D42B5FCC6D626&lat=" + lat + "&lng=" + lng + "\"");
    delay(10000);
    // Serial.println(F("waiting for url response"));
    while (gsm.available()) {
        Serial.write(gsm.read());
    }
    /*// response = getResponse(17 + 16 + 15 + 16 + 6 + lat.length() + lng.length() - 2);
    Serial.println(F("url res rcvd"));
    if (!response.equals("OK")) {
        initGSM();
        return;
    }*/
    
    Serial.println("exec action");
    //  send request
    // gsm.flush();
    sendToGSM("AT+HTTPACTION=1");
    delay(5000);
    response = getResponse(13);
    if (!response.equals("OKHTTPACTION12000")) {
        initGSM();
        return;
    }
}
