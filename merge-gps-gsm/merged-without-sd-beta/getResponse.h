String getResponse(short int skipLength) {
    char c;
    // skip first characters of length skipLength
    while (skipLength > 0) {
        if (gsm.available()) {
            c = (char)gsm.read();
            if (isAlphaNumeric(c)) {
                // Serial.print(c);
                skipLength--;
            }
        }
    }

   String res = "";
    while (gsm.available()) {
        c = (char) gsm.read();
        if (isAlphaNumeric(c))
           res.concat(c);
    }
    
    if (DEBUG) {Serial.print(F("response:")); Serial.println(res);}
    return res;
}
