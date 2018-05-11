String ConvertLat() {
    char posneg;
    if (nmea[2] == "S") {
        posneg = '-';
    }

    String latfirst = "";
    float latsecond = 0.0;

    for (short unsigned int i = 0; i < nmea[1].length(); i++) {
        if (nmea[1].substring(i, i + 1) == ".") {
            latfirst = nmea[1].substring(0, i - 2);
            latsecond = nmea[1].substring(i - 2).toFloat();
        }
    }

    latsecond = latsecond / 60;
    String CalcLat = "";

    char charVal[9];
    dtostrf(latsecond, 4, 6, charVal);
    for (short unsigned int i = 0; i < sizeof(charVal); i++) {
        CalcLat += charVal[i];
    }

    latfirst += CalcLat.substring(1);
    latfirst = (String)posneg += latfirst;
    return latfirst;
}


String ConvertLng() {
    char posneg;
    if (nmea[4] == "W") {
        posneg = '-';
    }

    String lngfirst = "";
    float lngsecond = 0.0;
    for (short unsigned int i = 0; i < nmea[3].length(); i++) {
        if (nmea[3].substring(i, i + 1) == ".") {
            lngfirst = nmea[3].substring(0, i - 2);
            //Serial.println(lngfirst);
            lngsecond = nmea[3].substring(i - 2).toFloat();
            //Serial.println(lngsecond);
        }
    }

    lngsecond = lngsecond / 60;
    String CalcLng = "";
    char charVal[9];
    dtostrf(lngsecond, 4, 6, charVal);

    for (short unsigned int i = 0; i < sizeof(charVal); i++) {
        CalcLng += charVal[i];
    }

    lngfirst += CalcLng.substring(1);
    lngfirst = (String)posneg += lngfirst;
    return lngfirst;
}
