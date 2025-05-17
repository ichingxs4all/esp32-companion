void setupLCD(){
lcd.init();                      // initialize the lcd 
lcd.backlight();
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Camera Shader");
lcd.setCursor(0,1);
lcd.print("Setting up WiFi");
}

void setupMenu(){
upBtn.begin();
downBtn.begin();
renderer.begin();
}