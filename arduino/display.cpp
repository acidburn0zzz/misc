// include the library code:
#include <LiquidCrystal.h>

void setup(LiquidCrystal *lcd) {
  // set up the LCD's number of columns and rows:
  lcd->begin(16, 2);
  // Print a message to the lcd->
  lcd->print("hello, world!");
}

void loop(LiquidCrystal *lcd) {
  lcd->setCursor(0, 0);
  lcd->print("hello, world!");
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd->setCursor(0, 1);
  // print the number of seconds since reset:
  lcd->print(millis()/1000);
}

int main() {
    int state = LOW;
    LiquidCrystal *lcd;

    init();

    // initialize the library with the numbers of the interface pins
    lcd = new LiquidCrystal(12, 11, 5, 4, 3, 2);
    pinMode(13, OUTPUT);

    setup(lcd);
    

    for (;;) {
        //loop(lcd);
        delay(500);
        
        if (state == LOW)
            state = HIGH;
        else
            state = LOW;

        digitalWrite(13, state);
    }

    return 0;
}
