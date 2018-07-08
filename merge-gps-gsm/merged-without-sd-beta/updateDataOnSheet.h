void updateDataOnSheet(String lat, String lng) {
    digitalWrite(5, HIGH);
    if (DEBUG) Serial.println(F("updating on sheet"));

    if (DEBUG) Serial.println(F("exec url"));
    String response = "";
    // make a request to url
    sendToGSMCharByChar("AT+HTTPPARA=\"URL\",\"<insert url here>&lat=" + lat + "&lng=" + lng + "\"");
    delay(20000);
    response = getResponse(35);
    if (!response.endsWith(F("OK"))) {
        initGSM();
        return;
    }
    
    if (DEBUG) Serial.println(F("exec action"));
    //  send request
    sendToGSMCharByChar(F("AT+HTTPACTION=1"));
    delay(10000);
    response = getResponse(13);
    if (!response.endsWith(F("OKHTTPACTION12000"))) {
        initGSM();
        return;
    }

    if (DEBUG) Serial.println(F("data sent"));
    digitalWrite(5, LOW);
}
