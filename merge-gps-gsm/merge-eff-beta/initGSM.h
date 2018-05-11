void initGSM() {
    String response = "";
    Serial.println(F("initialising gsm"));

    Serial.println(F("exec httpterm"));
    // terminate existing connection
    sendToGSM("AT+HTTPTERM");
    delay(2000);
    response = getResponse(10);
    if (!(response.equals("OK") || response.equals("ERROR")))
        blinkForever();

    Serial.println(F("exec cpin"));
    // gsm.flush();
    sendToGSM("AT+CPIN?");
    delay(2000);
    response = getResponse(10);
    if (!response.equals("READYOK"))
        blinkForever();

    Serial.println(F("exec cipmode"));
    // gsm.flush();
    // set non-transparent mode
    sendToGSM("AT+CIPMODE=0");
    delay(3000);
    response = getResponse(10);
    if (!response.equals("OK"))
        blinkForever();
    
    Serial.println(F("exec cipmux"));
    // gsm.flush();
    // set single connection mode
    sendToGSM("AT+CIPMUX=0");
    delay(3000);
    response = getResponse(9);
    if (!response.equals("OK"))
        blinkForever();

    Serial.println(F("exec cgatt"));
    // gsm.flush();
    // enable GPRS
    sendToGSM("AT+CGATT=1");
    delay(10000);
    response = getResponse(8);
    if (!response.equals("OK"))
        blinkForever();

    Serial.println(F("exec sap31gprs"));
    // gsm.flush();
    // set connection type to GPRS
    sendToGSM("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
    delay(10000);
    response = getResponse(20);
    if (!response.equals("OK"))
        blinkForever();

    Serial.println(F("exec sap31apn"));
    // gsm.flush();
    // set APN according to your network provider
    sendToGSM("AT+SAPBR=3,1,\"APN\",\"internet\"");
    delay(10000);
    response = getResponse(20);
    if (!response.equals("OK"))
        blinkForever();
    
    Serial.println(F("exec sap11"));
    // gsm.flush();
    sendToGSM("AT+SAPBR=1,1");
    delay(15000);
    response = getResponse(9);
    if (!response.equals("OK") && !response.equals("ERROR"))
        blinkForever();

    Serial.println(F("exec sap21"));
    // gsm.flush();
    sendToGSM("AT+SAPBR=2,1");
    delay(2000);
    getResponse(9);
    delay(3000);

    Serial.println(F("exec httpinit"));
    // initialise HTTP connection
    // gsm.flush();
    sendToGSM("AT+HTTPINIT");
    delay(3000);
    response = getResponse(10);
    if (!response.equals("OK"))
        blinkForever();
    
    Serial.println(F("exec cid"));
    // gsm.flush();
    // set barrier profile
    sendToGSM(F("AT+HTTPPARA=\"CID\",1"));
    delay(3000);
    response = getResponse(14);
    if (!response.equals("OK"))
        blinkForever();
    
    updateDataOnSheet("init", "done");
    Serial.println(F("initialisation done!"));
}
