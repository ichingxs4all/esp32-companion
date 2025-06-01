void setupEncoders_Buttons(){
 //Display MCP setup status on fifth led
setLedBlue(4);

  // === MCP1 SETUP ===
  if (!mcp1.begin_I2C(0x20)) {
    if(debug)Serial.println("MCP1 not found!");
    lcd.setCursor(0,2);
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
    lcd.setCursor(0,2);
    lcd.print("MCP2 not found!");
    while (1);
  }

  for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
    mcp2.pinMode(mcp2ButtonPins[i], INPUT_PULLUP);
    mcp2.setupInterruptPin(mcp2ButtonPins[i], CHANGE);
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
        if (b != a) 
        {
          //encoderPositions[i]++;
          switch (i) {
          case 0:
          if(setIris)msgRotRight=msgC1_R0_rotRight;
          if(setGain)msgRotRight=msgC1_R1_rotRight;
          if(setWB)msgRotRight=msgC1_R2_rotRight;
          if(setFocus)msgRotRight=msgC1_R3_rotRight;
          if(setZoom)msgRotRight=msgC1_R4_rotRight;
          break;
          case 1:
          if(setIris)msgRotRight=msgC2_R0_rotRight;
          if(setGain)msgRotRight=msgC2_R1_rotRight;
          if(setWB)msgRotRight=msgC2_R2_rotRight;
          if(setFocus)msgRotRight=msgC2_R3_rotRight;
          if(setZoom)msgRotRight=msgC2_R4_rotRight;
          break;
          case 2:
          if(setIris)msgRotRight=msgC3_R0_rotRight;
          if(setGain)msgRotRight=msgC3_R1_rotRight;
          if(setWB)msgRotRight=msgC3_R2_rotRight;
          if(setFocus)msgRotRight=msgC3_R3_rotRight;
          if(setZoom)msgRotRight=msgC3_R4_rotRight;
          break;
          case 3:
          if(setIris)msgRotRight=msgC4_R0_rotRight;
          if(setGain)msgRotRight=msgC4_R1_rotRight;
          if(setWB)msgRotRight=msgC4_R2_rotRight;
          if(setFocus)msgRotRight=msgC4_R3_rotRight;
          if(setZoom)msgRotRight=msgC4_R4_rotRight;
          break;
          case 4:
          if(setIris)msgRotRight=msgC5_R0_rotRight;
          if(setGain)msgRotRight=msgC5_R1_rotRight;
          if(setWB)msgRotRight=msgC5_R2_rotRight;
          if(setFocus)msgRotRight=msgC5_R3_rotRight;
          if(setZoom)msgRotRight=msgC5_R4_rotRight;
          break;
          case 5:
          if(setIris)msgRotRight=msgC6_R0_rotRight;
          if(setGain)msgRotRight=msgC6_R1_rotRight;
          if(setWB)msgRotRight=msgC6_R2_rotRight;
          if(setFocus)msgRotRight=msgC6_R3_rotRight;
          if(setZoom)msgRotRight=msgC6_R4_rotRight;
          break;
          default:
          if(setIris)msgRotRight=msgC1_R0_rotRight;
          if(setGain)msgRotRight=msgC1_R1_rotRight;
          if(setWB)msgRotRight=msgC1_R2_rotRight;
          if(setFocus)msgRotRight=msgC1_R3_rotRight;
          if(setZoom)msgRotRight=msgC1_R4_rotRight;
          break;
          }
          sendOSC_RotRight();
          }
        else {
          //encoderPositions[i]--;
          switch (i) {
          case 0:
          if(setIris)msgRotLeft=msgC1_R0_rotLeft;
          if(setGain)msgRotLeft=msgC1_R1_rotLeft;
          if(setWB)msgRotLeft=msgC1_R2_rotLeft;
          if(setFocus)msgRotLeft=msgC1_R3_rotLeft;
          if(setZoom)msgRotLeft=msgC1_R4_rotLeft;
          break;
          case 1:
          if(setIris)msgRotLeft=msgC2_R0_rotLeft;
          if(setGain)msgRotLeft=msgC2_R1_rotLeft;
          if(setWB)msgRotLeft=msgC2_R2_rotLeft;
          if(setFocus)msgRotLeft=msgC2_R3_rotLeft;
          if(setZoom)msgRotLeft=msgC2_R4_rotLeft;
          break;
          case 2:
          if(setIris)msgRotLeft=msgC3_R0_rotLeft;
          if(setGain)msgRotLeft=msgC3_R1_rotLeft;
          if(setWB)msgRotLeft=msgC3_R2_rotLeft;
          if(setFocus)msgRotLeft=msgC3_R3_rotLeft;
          if(setZoom)msgRotLeft=msgC3_R4_rotLeft;
          break;
          case 3:
          if(setIris)msgRotLeft=msgC4_R0_rotLeft;
          if(setGain)msgRotLeft=msgC4_R1_rotLeft;
          if(setWB)msgRotLeft=msgC4_R2_rotLeft;
          if(setFocus)msgRotLeft=msgC4_R3_rotLeft;
          if(setZoom)msgRotLeft=msgC4_R4_rotLeft;
          break;
          case 4:
          if(setIris)msgRotLeft=msgC5_R0_rotLeft;
          if(setGain)msgRotLeft=msgC5_R1_rotLeft;
          if(setWB)msgRotLeft=msgC5_R2_rotLeft;
          if(setFocus)msgRotLeft=msgC5_R3_rotLeft;
          if(setZoom)msgRotLeft=msgC5_R4_rotLeft;
          break;
          case 5:
          if(setIris)msgRotLeft=msgC6_R0_rotLeft;
          if(setGain)msgRotLeft=msgC6_R1_rotLeft;
          if(setWB)msgRotLeft=msgC6_R2_rotLeft;
          if(setFocus)msgRotLeft=msgC6_R3_rotLeft;
          if(setZoom)msgRotLeft=msgC6_R4_rotLeft;
          break;
          default:
          if(setIris)msgRotLeft=msgC1_R0_rotLeft;
          if(setGain)msgRotLeft=msgC1_R1_rotLeft;
          if(setWB)msgRotLeft=msgC1_R2_rotLeft;
          if(setFocus)msgRotLeft=msgC1_R3_rotLeft;
          if(setZoom)msgRotLeft=msgC1_R4_rotLeft;
          break;
          }
          sendOSC_RotLeft();
          }
        //onEncoderMoved(i, encoderPositions[i]);
        lastAState[i] = a;
      }

      if (btn != lastButtonState[i] && now - lastBtnDebounceTime[i] > DEBOUNCE_DELAY_MS) {
        lastBtnDebounceTime[i] = now;
        if (!btn) {
          //onEncoderButtonPressed(i);
          switch (i) {
          case 0:
          if(setIris)msgPress=msgC1_R0_press;
          if(setGain)msgPress=msgC1_R1_press;
          if(setWB)msgPress=msgC1_R2_press;
          if(setFocus)msgPress=msgC1_R3_press;
          if(setZoom)msgPress=msgC1_R4_press;
          break;
          case 1:
          if(setIris)msgPress=msgC2_R0_press;
          if(setGain)msgPress=msgC2_R1_press;
          if(setWB)msgPress=msgC2_R2_press;
          if(setFocus)msgPress=msgC2_R3_press;
          if(setZoom)msgPress=msgC2_R4_press;
          break;
          case 2:
          if(setIris)msgPress=msgC3_R0_press;
          if(setGain)msgPress=msgC3_R1_press;
          if(setWB)msgPress=msgC3_R2_press;
          if(setFocus)msgPress=msgC3_R3_press;
          if(setZoom)msgPress=msgC3_R4_press;
          break;
          case 3:
          if(setIris)msgPress=msgC4_R0_press;
          if(setGain)msgPress=msgC4_R1_press;
          if(setWB)msgPress=msgC4_R2_press;
          if(setFocus)msgPress=msgC4_R3_press;
          if(setZoom)msgPress=msgC4_R4_press;
          break;
          case 4:
          if(setIris)msgPress=msgC5_R0_press;
          if(setGain)msgPress=msgC5_R1_press;
          if(setWB)msgPress=msgC5_R2_press;
          if(setFocus)msgPress=msgC5_R3_press;
          if(setZoom)msgPress=msgC5_R4_press;
          break;
          case 5:
          if(setIris)msgPress=msgC6_R0_press;
          if(setGain)msgPress=msgC6_R1_press;
          if(setWB)msgPress=msgC6_R2_rotLeft;
          if(setFocus)msgPress=msgC6_R3_press;
          if(setZoom)msgPress=msgC6_R4_press;
          break;
          default:
          if(setIris)msgPress=msgC1_R0_press;
          if(setGain)msgPress=msgC1_R1_press;
          if(setWB)msgPress=msgC1_R2_press;
          if(setFocus)msgPress=msgC1_R3_press;
          if(setZoom)msgPress=msgC1_R4_press;
          break;
          }
          sendOSC_Press();
        }
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