// === CALLBACK FUNCTIONS ===
void onEncoderMoved(uint8_t id, int newPosition) {
  if(debug)Serial.printf("Encoder %d moved to %d\n", id, newPosition);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Camera");
  lcd.setCursor(0,1);
  lcd.print(id);
  lcd.setCursor(0,2);
  lcd.print("Moved to");
  lcd.setCursor(0,3);
  lcd.print(newPosition);
}

void onEncoderButtonPressed(uint8_t id) {
  if(debug)Serial.printf("Encoder %d button pressed\n", id);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Camera");
  lcd.setCursor(0,1);
  lcd.print(id);
  lcd.setCursor(0,2);
  lcd.print("Selected");
}

void onMCP2ButtonPressed(uint8_t id) {
  if(debug)Serial.printf("MCP2 Button %d pressed\n", id);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Camera");
  lcd.setCursor(0,1);
  lcd.print(id);
  lcd.setCursor(0,2);
  lcd.print("Selected");

  for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
    mcp2.digitalWrite(mcp2LEDPins[i], (i == id) ? HIGH : LOW);
  }
}

