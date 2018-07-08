void updateDataOnSheet(String lat, String lng) {
    digitalWrite(5, HIGH);
    // Serial.println(F("updating on sheet"));
    
    String response = "";
    // Serial.println(F("exec url"));
    // make a request to url
    // gsm.flush();
    sendToGSMCharByChar("AT+HTTPPARA=\"URL\",\"<insert url here>&lat=" + lat + "&lng=" + lng + "\"");
    delay(20000);
    response = getResponse(35);
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

    // Serial.println(F("data sent"));
    digitalWrite(5, LOW);
}
