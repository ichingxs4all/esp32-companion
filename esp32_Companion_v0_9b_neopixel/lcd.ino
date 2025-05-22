void setupLCD(){
//Display LCD setup status on second led led
setLedBlue(1);

lcd.init();                      // initialize the lcd 
lcd.backlight();
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Camera Shader v0.9");
lcd.setCursor(0,1);
lcd.print("Setting up WiFi");

}

void setupMenu(){
//Display menu setup status on sixt led
setLedBlue(5);

enterBtn.begin();
backBtn.begin();
renderer.begin();
}