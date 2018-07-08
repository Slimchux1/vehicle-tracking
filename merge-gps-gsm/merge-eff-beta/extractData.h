// extract data from output string of GPS
void extractData() {
    short int stringStartIndex = 0, stringEndIndex = 0;

    String tempMsg = GPSModule.readStringUntil('\n');
    // Serial.println(tempMsg);
    // extract meaningful data
    for (short unsigned int i = 0; i < SIZE; i++) {
        // find index where comma is
        while (tempMsg[stringEndIndex] != ',')
            stringEndIndex++;

        // copy substring of tempMsg from startIndex to EndIndex
        nmea[i] = tempMsg.substring(stringStartIndex, stringEndIndex);
        stringStartIndex = stringEndIndex + 1;
        stringEndIndex++;
    }
}
