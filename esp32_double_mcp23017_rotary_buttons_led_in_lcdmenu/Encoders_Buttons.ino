void setupEncoders_Buttons(){

  // === MCP1 SETUP ===
  if (!mcp1.begin_I2C(0x20)) {
    if(debug)Serial.println("MCP1 not found!");
    lcd.setCursor(0,0);
    lcd.print("MCP1 not found!");
    while (1);
  }

  for (uint8_t i = 0; i < NUM_ENCODERS; i++) {
    mcp1.pinMode(encoderPins[i][0], INPUT_PULLUP);
    mcp1.pinMode(encoderPins[i][1], INPUT_PULLUP);
    mcp1.pinMode(encoderPins[i][2], INPUT_PULLUP);

    mcp1.setupInterruptPin(encoderPins[i][0], CHANGE);
    mcp1.setupInterruptPin(encoderPins[i][1], CHANGE);
    mcp1.setupInterruptPin(encoderPins[i][2], CHANGE);

    lastAState[i] = mcp1.digitalRead(encoderPins[i][0]);
    lastButtonState[i] = mcp1.digitalRead(encoderPins[i][2]);
  }

  pinMode(MCP1_INT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(MCP1_INT_PIN), onMCP1Interrupt, FALLING);

  // === MCP2 SETUP ===
  if (!mcp2.begin_I2C(0x21)) {
    if(debug)Serial.println("MCP2 not found!");
    lcd.setCursor(0,1);
    lcd.print("MCP2 not found!");
    while (1);
  }

  for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
    mcp2.pinMode(mcp2ButtonPins[i], INPUT_PULLUP);
    mcp2.setupInterruptPin(mcp2ButtonPins[i], CHANGE);
    mcp2.pinMode(mcp2LEDPins[i], OUTPUT);
    mcp2.digitalWrite(mcp2LEDPins[i], HIGH); //Little led annimation
    delay(100);
    mcp2.digitalWrite(mcp2LEDPins[i], LOW);
    lastButtonState2[i] = mcp2.digitalRead(mcp2ButtonPins[i]);
  }

  pinMode(MCP2_INT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(MCP2_INT_PIN), onMCP2Interrupt, FALLING);

}



void checkEncoders_Buttons(){

  unsigned long now = millis();

  // === MCP1: Encoders ===
  if (mcp1InterruptFlag) {
    mcp1InterruptFlag = false;
    mcp1.getLastInterruptPin(); // clear
    //mcp1.getLastInterruptPinValue();

    for (uint8_t i = 0; i < NUM_ENCODERS; i++) {
      bool a = mcp1.digitalRead(encoderPins[i][0]);
      bool b = mcp1.digitalRead(encoderPins[i][1]);
      bool btn = mcp1.digitalRead(encoderPins[i][2]);

      if (a != lastAState[i] && now - lastADebounceTime[i] > DEBOUNCE_DELAY_MS) {
        lastADebounceTime[i] = now;
        if (b != a) encoderPositions[i]++;
        else encoderPositions[i]--;
        onEncoderMoved(i, encoderPositions[i]);
        lastAState[i] = a;
      }

      if (btn != lastButtonState[i] && now - lastBtnDebounceTime[i] > DEBOUNCE_DELAY_MS) {
        lastBtnDebounceTime[i] = now;
        if (!btn) onEncoderButtonPressed(i);
        lastButtonState[i] = btn;
      }
    }
  }

  // === MCP2: Buttons ===
  if (mcp2InterruptFlag) {
    mcp2InterruptFlag = false;
    mcp2.getLastInterruptPin();
    //mcp2.getLastInterruptPinValue();

    for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
      bool currentState = mcp2.digitalRead(mcp2ButtonPins[i]);

      if (currentState != lastButtonState2[i] && now - lastButton2DebounceTime[i] > DEBOUNCE_DELAY_MS) {
        lastButton2DebounceTime[i] = now;
        if (!currentState) onMCP2ButtonPressed(i);
        lastButtonState2[i] = currentState;
      }
    }
  }

  delay(1); // prevent CPU starvation
}