void initGSM() {
    String response = "";
    digitalWrite(5, LOW);
    Serial.println(F("initialising gsm"));

    // Serial.println(F("exec httpterm"));
    // terminate existing connection
    sendToGSMCharByChar(F("AT+HTTPTERM"));
    digitalWrite(5, HIGH);
    delay(2000);
    digitalWrite(5, LOW);
    response = getResponse(10);
    if (!(response.endsWith(F("OK")) || response.endsWith(F("ERROR"))))
        blinkForever();

    // Serial.println(F("exec cpin"));
    // gsm.flush();
    sendToGSMCharByChar(F("AT+CPIN?"));
    delay(2000);
    digitalWrite(5, HIGH);
    response = getResponse(10);
    if (!response.endsWith(F("READYOK")))
        blinkForever();

    // Serial.println(F("exec cipmode"));
    // gsm.flush();
    // set non-transparent mode
    sendToGSMCharByChar(F("AT+CIPMODE=0"));
    delay(3000);
    digitalWrite(5, LOW);
    response = getResponse(10);
    if (!response.endsWith(F("OK")))
        blinkForever();
    
    // Serial.println(F("exec cipmux"));
    // gsm.flush();
    // set single connection mode
    sendToGSMCharByChar(F("AT+CIPMUX=0"));
    delay(3000);
    digitalWrite(5, HIGH);
    response = getResponse(9);
    if (!response.endsWith(F("OK")))
        blinkForever();

    // Serial.println(F("exec cgatt"));
    // gsm.flush();
    // enable GPRS
    sendToGSMCharByChar(F("AT+CGATT=1"));
    delay(10000);
    digitalWrite(5, LOW);
    response = getResponse(8);
    if (!response.endsWith(F("OK")))
        blinkForever();

    // Serial.println(F("exec sap31gprs"));
    // gsm.flush();
    // set connection type to GPRS
    sendToGSMCharByChar(F("AT+SAPBR=3,1,\"Contype\",\"GPRS\""));
    delay(10000);
    digitalWrite(5, HIGH);
    response = getResponse(20);
    if (!response.endsWith(F("OK")))
        blinkForever();

    // Serial.println(F("exec sap31apn"));
    // gsm.flush();
    // set APN according to your network provider
    sendToGSMCharByChar(F("AT+SAPBR=3,1,\"APN\",\"internet\""));
    delay(10000);
    digitalWrite(5, LOW);
    response = getResponse(20);
    if (!response.endsWith(F("OK")))
        blinkForever();
    
    // Serial.println(F("exec sap11"));
    // gsm.flush();
    sendToGSMCharByChar(F("AT+SAPBR=1,1"));
    delay(15000);
    digitalWrite(5, HIGH);
    response = getResponse(9);
    if (!response.endsWith(F("OK")) && !response.endsWith(F("ERROR")))
        blinkForever();

    // Serial.println(F("exec sap21"));
    // gsm.flush();
    sendToGSMCharByChar(F("AT+SAPBR=2,1"));
    delay(2000);
    digitalWrite(5, LOW);
    getResponse(9);
    delay(3000);

    // Serial.println(F("exec httpinit"));
    // initialise HTTP connection
    // gsm.flush();
    sendToGSMCharByChar(F("AT+HTTPINIT"));
    delay(3000);
    digitalWrite(5, HIGH);
    response = getResponse(10);
    if (!response.endsWith(F("OK")))
        blinkForever();
    
    // Serial.println(F("exec cid"));
    // gsm.flush();
    // set barrier profile
    sendToGSMCharByChar(F("AT+HTTPPARA=\"CID\",1"));
    delay(3000);
    digitalWrite(5, LOW);
    response = getResponse(14);
    if (!response.endsWith(F("OK")))
        blinkForever();

    updateDataOnSheet(F("init"), F("done"));
    Serial.println(F("initialisation done!"));
    digitalWrite(5, LOW);
}
