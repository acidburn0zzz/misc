int main() {
    int ledPin = 9;

    /* DO NOT REMOVE */
    init();

    /* Main loop */
    for (;;) {
        for (int i=0; i<=255; i+=5) {
            analogWrite(ledPin, i);
            delay(30);
        }

        for (int i=255; i>=0; i-=5) {
            analogWrite(ledPin, i);
            delay(30);
        }
    }

    return 0;
}
