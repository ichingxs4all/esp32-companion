// === CALLBACK FUNCTIONS ===
void onEncoderMoved(uint8_t id, int newPosition) {
  if(debug)Serial.printf("Encoder %d moved to %d\n", id, newPosition);
}

void onEncoderButtonPressed(uint8_t id) {
  if(debug)Serial.printf("Encoder %d button pressed\n", id);
}

void onMCP2ButtonPressed(uint8_t id) {
  //if(debug)Serial.printf("MCP2 Button %d pressed\n", id);
  switch (id){
  case 0:
  msgPress=msgC1_R5_press;
  break;
  case 1:
  msgPress=msgC2_R5_press;
  break;
   case 2:
  msgPress=msgC3_R5_press;
  break;
   case 3:
  msgPress=msgC4_R5_press;
  break;
   case 4:
  msgPress=msgC5_R5_press;
  break;
   case 5:
  msgPress=msgC6_R5_press;
  break;
  default:
  break;
  }
sendOSC_Press();

//delay(100);

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
//lcd.setCursor(0,2);
//lcd.print("Gain mode.   ");
}

void toggleIris(){
setGain = false;
setIris = true;
setWB = false;
setFocus = false;
setZoom = false;
 if(debug)Serial.println("Iris mode");
//lcd.setCursor(0,2);
//lcd.print("Iris mode    ");
}

void toggleWB(){
setGain = false;
setIris = false;
setWB = true;
setFocus = false;
setZoom = false;
if(debug)Serial.println("WB mode");
//lcd.setCursor(0,2);
//lcd.print("WB mode      ");
}


void toggleFocus(){
setGain = false;
setIris = false;
setWB = false;
setFocus = true;
setZoom = false;
if(debug)Serial.println("Focus mode");
//lcd.setCursor(0,2);
//lcd.print("Focus mode   ");
}

void toggleZoom(){
setGain = false;
setIris = false;
setWB = false;
setFocus = false;
setZoom = true;
if(debug)Serial.println("Zoom mode");
//lcd.setCursor(0,2);
//lcd.print("Zoom mode    ");
}



