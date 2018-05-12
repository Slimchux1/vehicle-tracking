void updateDataOnSheet(String lat, String lng) {
    String response = "";
    // Serial.println(F("exec url"));
    // make a request to url
    // gsm.flush();
    sendToGSMCharByChar("AT+HTTPPARA=\"URL\",\"http://api.pushingbox.com/pushingbox?devid=v15D42B5FCC6D626&lat=" + lat + "&lng=" + lng + "\"");
    delay(20000);
    response = getResponse(33);
    /*if (!response.endsWith(F("OK"))) {
        initGSM();
        return;
    }*/
    
    // Serial.println(F("exec action"));
    //  send request
    // gsm.flush();
    sendToGSMCharByChar(F("AT+HTTPACTION=1"));
    delay(10000);
    response = getResponse(13);
    if (!response.endsWith(F("OKHTTPACTION12000"))) {
        initGSM();
        return;
    }
}
