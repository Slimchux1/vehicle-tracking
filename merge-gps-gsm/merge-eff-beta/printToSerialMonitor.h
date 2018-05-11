void printToSerialMonitor() {
    for (int i = 0; i < SIZE; i++) {
        Serial.print(nmea[i]);
        Serial.print(" ");
    }
    Serial.println("");
}
