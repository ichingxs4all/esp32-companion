#include <Adafruit_MCP23X08.h>
#include <Adafruit_MCP23X17.h>
#include <Adafruit_MCP23XXX.h>

#include <Wire.h>


Adafruit_MCP23X17 mcp0;
Adafruit_MCP23X17 mcp1;

volatile bool mcp0Interrupt = false;
volatile bool mcp1Interrupt = false;

#define MCP0_INT_PIN 34
#define MCP1_INT_PIN 35

// Encoder status
int encoderValues[6] = {0};
bool lastA[6], lastB[6], lastButton[6];

// Encoder pin mapping
uint8_t encoderPins[6][3] = {
  {0, 1, 2}, {3, 4, 5}, {6, 7, 8},
  {0, 1, 2}, {3, 4, 5}, {6, 7, 8}
};

void IRAM_ATTR handleMCP0Interrupt() {
  mcp0Interrupt = true;
}

void IRAM_ATTR handleMCP1Interrupt() {
  mcp1Interrupt = true;
}

void setup() {
  Serial.begin(115200);
  Wire.begin();

  mcp0.begin_I2C(0x20);
  mcp1.begin_I2C(0x21);

  // Setup alle gebruikte pins als INPUT_PULLUP
  for (int i = 0; i < 9; i++) {
    mcp0.pinMode(i, INPUT_PULLUP);
    mcp1.pinMode(i, INPUT_PULLUP);
  }

  // Configure interrupts op alle pinnen
  for (int i = 0; i < 9; i++) {
    mcp0.setupInterruptPin(i, CHANGE);
    mcp1.setupInterruptPin(i, CHANGE);
  }

  // Setup ESP32 interrupt pins
  pinMode(MCP0_INT_PIN, INPUT_PULLUP);
  pinMode(MCP1_INT_PIN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(MCP0_INT_PIN), handleMCP0Interrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(MCP1_INT_PIN), handleMCP1Interrupt, FALLING);

  // Init vorige waardes
  for (int i = 0; i < 6; i++) {
    lastA[i] = readPinA(i);
    lastB[i] = readPinB(i);
    lastButton[i] = readButton(i);
  }
}

void loop() {
  if (mcp0Interrupt) {
    mcp0Interrupt = false;
    handleMCPInterrupt(mcp0, 0); // encoders 0-2
  }

  if (mcp1Interrupt) {
    mcp1Interrupt = false;
    handleMCPInterrupt(mcp1, 3); // encoders 3-5
  }

  delay(1);
}

void handleMCPInterrupt(Adafruit_MCP23X17 &mcp, int baseIndex) {
  uint8_t pin = mcp.getLastInterruptPin();
  if (pin == 255) return; // No interrupt

  //bool value = mcp.getLastInterruptPin();

  for (int i = 0; i < 3; i++) {
    int enc = baseIndex + i;
    bool A = readPinA(enc);
    bool B = readPinB(enc);

    if (A != lastA[enc]) {
      if (B != A)
        encoderValues[enc]++;
      else
        encoderValues[enc]--;

      Serial.print("Encoder ");
      Serial.print(enc);
      Serial.print(": ");
      Serial.println(encoderValues[enc]);
    }

    lastA[enc] = A;
    lastB[enc] = B;

    bool btn = readButton(enc);
    if (btn != lastButton[enc]) {
      if (!btn) {
        Serial.print("Button ");
        Serial.print(enc);
        Serial.println(" pressed");
      }
      lastButton[enc] = btn;
    }
  }
}

// Helper functies
bool readPinA(int enc) {
  return enc < 3 ? mcp0.digitalRead(encoderPins[enc][0]) : mcp1.digitalRead(encoderPins[enc][0]);
}
bool readPinB(int enc) {
  return enc < 3 ? mcp0.digitalRead(encoderPins[enc][1]) : mcp1.digitalRead(encoderPins[enc][1]);
}
bool readButton(int enc) {
  return enc < 3 ? mcp0.digitalRead(encoderPins[enc][2]) : mcp1.digitalRead(encoderPins[enc][2]);
}
