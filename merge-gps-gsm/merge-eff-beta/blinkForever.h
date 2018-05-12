void blinkForever() {
    Serial.println(F("blinking"));
    while (true) {
        digitalWrite(5, HIGH);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);  
        digitalWrite(LED_BUILTIN, LOW);
        digitalWrite(5, LOW);
        delay(100);
    }
}
