void sendToGSMCharByChar(String s) {
    gsm.begin(9600);
    int len = (int)s.length();
    for (short unsigned int i = 0; i < len; i++) {
        gsm.print(s[i]);
    }
    gsm.println("");
}
