void blinkForever() {
    Serial.println(F("blinking"));
    while (true) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(1000);  
        digitalWrite(LED_BUILTIN, LOW);
        delay(1000);
    }
}