void setupButtonRotary(){

  r.begin(ROTARY_PIN1, ROTARY_PIN2, CLICKS_PER_STEP);
  r.setChangedHandler(rotate);
  r.setLeftRotationHandler(decrement);
  r.setRightRotationHandler(increment);

  b.begin(BUTTON_PIN);
  b.setTapHandler(click);
  b.setLongClickHandler(resetPosition);

}

// on change
void rotate(ESPRotary& r) {
   
   positionA = r.getPosition();
   if(debug)Serial.println(positionA);
}

// on left or right rotation
void showDirection(ESPRotary& r) {
  //direction = r.getDirection();
  //Serial.println(direction);
  if(debug)Serial.println(r.directionToString(r.getDirection()));
}

void increment(ESPRotary& r){
  if(debug)Serial.println("Increment");
  sendOSCC();
}

void decrement(ESPRotary& r){
  if(debug)Serial.println("Decrement");
  sendOSCB();
}

// single click
void click(Button2& btn) {
  if(debug)Serial.println("Click!");
  buttonPress = true;
}

// long click
void resetPosition(Button2& btn) {
  r.resetPosition();
  if(debug)Serial.println("Reset!");
}
/////////////////////////////////////////////////////////////////
