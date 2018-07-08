void printToSerialMonitor() {
    for (int unsigned i = 0; i < SIZE; i++) {
        Serial.print(nmea[i]);
        Serial.print(" ");
    }
    Serial.println("");
}
