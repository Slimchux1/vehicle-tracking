String ConvertLat() {
    String posneg = "";
    if (nmea[2] == "S") {
        posneg = "-";
    }

    String latfirst = "";
    float latsecond = 0.0;

    for (short unsigned int i = 0, len = nmea[1].length(); i < len; i++) {
        if (nmea[1][i] == '.') {
            latfirst = nmea[1].substring(0, i - 2);
            latsecond = nmea[1].substring(i - 2).toFloat();
        }
    }

    latsecond = latsecond / 60;
    String CalcLat = "";

    char charVal[9];
    dtostrf(latsecond, 4, 6, charVal);
    CalcLat.concat(charVal);

    latfirst.concat(CalcLat.substring(1));
    latfirst = posneg += latfirst;
    return latfirst;
}


String ConvertLng() {
    String posneg = "";
    if (nmea[4] == "W") {
        posneg = "-";
    }

    String lngfirst = "";
    float lngsecond = 0.0;
    for (short unsigned int i = 0, len = nmea[3].length(); i < len; i++) {
        if (nmea[3][i] == '.') {
            lngfirst = nmea[3].substring(0, i - 2);
            lngsecond = nmea[3].substring(i - 2).toFloat();
        }
    }

    lngsecond = lngsecond / 60;
    String CalcLng = "";
    
    char charVal[9];
    dtostrf(lngsecond, 4, 6, charVal);
    CalcLng.concat(charVal);

    lngfirst.concat(CalcLng.substring(1));
    lngfirst = posneg += lngfirst;
    return lngfirst;
}
