void initGSM() {
    digitalWrite(5, HIGH);
    delay(100);
    digitalWrite(5, LOW);
    delay(100);
    digitalWrite(5, HIGH);
    delay(100);
    digitalWrite(5, LOW);
    delay(100);
    digitalWrite(5, HIGH);    

    if (DEBUG) Serial.println(F("initialising gsm"));
    String response = "";

    if (DEBUG) Serial.println(F("exec httpterm"));
    // terminate existing connection
    sendToGSMCharByChar(F("AT+HTTPTERM"));
    delay(500);
    response = getResponse(10);
    if (!(response.endsWith(F("OK")) || response.endsWith(F("ERROR"))))
        blinkForever();

    if (DEBUG) Serial.println(F("exec cpin"));
    sendToGSMCharByChar(F("AT+CPIN?"));
    delay(500);
    response = getResponse(10);
    if (!response.endsWith(F("READYOK")))
        blinkForever();

    if (DEBUG) Serial.println(F("exec cipmode"));
    // set non-transparent mode
    sendToGSMCharByChar(F("AT+CIPMODE=0"));
    delay(3000);
    response = getResponse(10);
        blinkForever();
    
    if (DEBUG) Serial.println(F("exec cipmux"));
    // set single connection mode
    sendToGSMCharByChar(F("AT+CIPMUX=0"));
    delay(3000);
    response = getResponse(9);
    if (!response.endsWith(F("OK")))
        blinkForever();

    if (DEBUG) Serial.println(F("exec cgatt"));
    // enable GPRS
    sendToGSMCharByChar(F("AT+CGATT=1"));
    delay(10000);
    response = getResponse(8);
    if (!response.endsWith(F("OK")))
        blinkForever();

    if (DEBUG) Serial.println(F("exec sap31gprs"));
    // set connection type to GPRS
    sendToGSMCharByChar(F("AT+SAPBR=3,1,\"Contype\",\"GPRS\""));
    delay(10000);
    response = getResponse(20);
    if (!response.endsWith(F("OK")))
        blinkForever();

    if (DEBUG) Serial.println(F("exec sap31apn"));
    // set APN according to your network provider
    sendToGSMCharByChar(F("AT+SAPBR=3,1,\"APN\",\"bsnlnet\""));
    delay(5000);
    response = getResponse(19);
    if (!response.endsWith(F("OK")))
        blinkForever();
    
    if (DEBUG) Serial.println(F("exec sap11"));
    sendToGSMCharByChar(F("AT+SAPBR=1,1"));
    delay(7000);
    response = getResponse(9);
    if (!response.endsWith(F("OK")) && !response.endsWith(F("ERROR")))
        blinkForever();

    if (DEBUG) Serial.println(F("exec sap21"));
    sendToGSMCharByChar(F("AT+SAPBR=2,1"));
    delay(2000);
    getResponse(9);
    
    if (DEBUG) Serial.println(F("exec httpinit"));
    // initialise HTTP connection
    sendToGSMCharByChar(F("AT+HTTPINIT"));
    delay(1000);
    response = getResponse(10);
    if (!response.endsWith(F("OK")))
        blinkForever();
    
    if (DEBUG) Serial.println(F("exec cid"));
    // set barrier profile
    sendToGSMCharByChar(F("AT+HTTPPARA=\"CID\",1"));
    delay(1000);
    response = getResponse(14);
    if (!response.endsWith(F("OK")))
        blinkForever();

    updateDataOnSheet(F("init"), F("done"));
    if (DEBUG) Serial.println(F("initialisation done!"));
    
    digitalWrite(5, LOW);
}
