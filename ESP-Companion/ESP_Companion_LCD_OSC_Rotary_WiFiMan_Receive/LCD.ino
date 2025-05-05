void setupLCD(){
     // set up the LCD's number of columns and rows, must be called.
    lcd.begin(16, 2);
    
    // most backpacks have the backlight on pin 3.
    lcd.configureBacklightPin(3);
    lcd.backlight();
    
    lcd.clear();
    // Print a message to the LCD.
    lcd.print("ESP-Companion");
}