void setupLeds(){
  pixels.begin();
  //Display neopixel led setup status on first led
  setLedBlue(0);
}

void setLedRed(int i){
 //pixels.clear();
 pixels.setPixelColor(i, pixels.Color(150, 0, 0));
 pixels.show();
}

void setLedGreen(int i){
 pixels.setPixelColor(i, pixels.Color(0, 150, 0));
 pixels.show();
}

void setLedBlack(int i){
 pixels.setPixelColor(i, pixels.Color(0, 0, 0));
 pixels.show();
}

void setLedBlue(int i){
 //pixels.clear();
 pixels.setPixelColor(i, pixels.Color(0, 0, 150));
 pixels.show();
}

void clearAllLeds(){
  pixels.clear();
  pixels.show();
}