// === CALLBACK FUNCTIONS ===
void onEncoderMoved(uint8_t id, int newPosition) {
  if(debug)Serial.printf("Encoder %d moved to %d\n", id, newPosition);
  //lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" Camera:");
  lcd.setCursor(8,0);
  lcd.print(id+1);
  lcd.setCursor(10,0);
  //lcd.print("Moved to");
  //lcd.setCursor(0,3);
  lcd.print(newPosition);
  lcd.print("         ");
}

void onEncoderButtonPressed(uint8_t id) {
  if(debug)Serial.printf("Encoder %d button pressed\n", id);
  //lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" Camera:");
  lcd.setCursor(8,0);
  lcd.print(id+1);
  lcd.setCursor(10,0);
  lcd.print("Setting Saved");
}

void onMCP2ButtonPressed(uint8_t id) {
  //if(debug)Serial.printf("MCP2 Button %d pressed\n", id);
  switch (id){
  case 0:
  msgPress=msgC1_R5_press;
  sendOSC_Press();
  break;
  case 1:
  msgPress=msgC2_R5_press;
  sendOSC_Press();
  break;
   case 2:
  msgPress=msgC2_R5_press;
  sendOSC_Press();
  break;
   case 3:
  msgPress=msgC2_R5_press;
  sendOSC_Press();
  break;
   case 4:
  msgPress=msgC2_R5_press;
  sendOSC_Press();
  break;
   case 5:
  msgPress=msgC2_R5_press;
  sendOSC_Press();
  break;
  default:
  break;
  }
  
  //lcd.clear();
  //lcd.setCursor(0,0);
  //lcd.print(" Camera:");
  //lcd.setCursor(8,0);
  //lcd.print(id+1);
  //lcd.setCursor(10,0);
  //lcd.print("Actual");
  
delay(100);
  for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
    //mcp2.digitalWrite(mcp2LEDPins[i], (i == id) ? HIGH : LOW);
  }
}


// Define the callbacks
void toggleBacklight(bool isOn) {
    lcdAdapter.setBacklight(isOn);
}

void inputCallback(char* value) {
    Serial.println(value);
}

void toggleGain(){
setGain = true;
setIris = false;
setWB = false;
setFocus = false;
setZoom = false;
if(debug)Serial.println("Gain mode");
lcd.setCursor(0,2);
lcd.print("Gain mode.   ");
}

void toggleIris(){
setGain = false;
setIris = true;
setWB = false;
setFocus = false;
setZoom = false;
 if(debug)Serial.println("Iris mode");
lcd.setCursor(0,2);
lcd.print("Iris mode    ");
}

void toggleWB(){
setGain = false;
setIris = false;
setWB = true;
setFocus = false;
setZoom = false;
if(debug)Serial.println("WB mode");
lcd.setCursor(0,2);
lcd.print("WB mode      ");
}


void toggleFocus(){
setGain = false;
setIris = false;
setWB = false;
setFocus = true;
setZoom = false;
if(debug)Serial.println("Focus mode");
lcd.setCursor(0,2);
lcd.print("Focus mode   ");
}

void toggleZoom(){
setGain = false;
setIris = false;
setWB = false;
setFocus = false;
setZoom = true;
if(debug)Serial.println("Zoom mode");
lcd.setCursor(0,2);
lcd.print("Zoom mode    ");
}



