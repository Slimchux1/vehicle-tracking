void writeToSD() {
    // Serial.println("\n<<<<<<<<<<<Lat:" + nmea[1] + "  Lng:" + nmea[3] + ">>>>>>>>>>>>>\n");

    File dataFile = SD.open("MERGETEST10MAY.CSV", FILE_WRITE);
    // write if dataFile is succesfully created/opened
    if (dataFile) {
        // Serial.println("writing");

        // time
        dataFile.print(nmea[0]);
        dataFile.print(",");

        // latitude
        dataFile.print(nmea[1]);
        dataFile.print(",");

        // longitude
        dataFile.print(nmea[3]);
        dataFile.println("");

        // Serial.println("writing ends!");
    }
    else {
        // Serial.println("error");
    }

    dataFile.close();
}
