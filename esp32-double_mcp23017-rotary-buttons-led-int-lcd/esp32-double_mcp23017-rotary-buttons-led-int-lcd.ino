#include <Wire.h>
#include <Adafruit_MCP23X17.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

Adafruit_MCP23X17 mcp1; // Encoders
Adafruit_MCP23X17 mcp2; // Buttons + LEDs

// === CONFIGURATION ===
const uint8_t MCP1_INT_PIN = 34; // INT from mcp1
const uint8_t MCP2_INT_PIN = 32; // INT from mcp2
const unsigned long DEBOUNCE_DELAY_MS = 10;

// === MCP1: Encoders ===
const uint8_t NUM_ENCODERS = 5;
const uint8_t encoderPins[NUM_ENCODERS][3] = {
  {0, 1, 2},   // A, B, Button
  {3, 4, 5},
  {6, 7, 8},
  {9, 10, 11},
  {12, 13, 14}
};

int encoderPositions[NUM_ENCODERS] = {0};
bool lastAState[NUM_ENCODERS];
bool lastButtonState[NUM_ENCODERS];
unsigned long lastADebounceTime[NUM_ENCODERS] = {0};
unsigned long lastBtnDebounceTime[NUM_ENCODERS] = {0};

// === MCP2: Buttons and LEDs ===
const uint8_t NUM_BUTTONS = 6;
const uint8_t mcp2ButtonPins[NUM_BUTTONS] = {0, 1, 2, 3, 4, 5};      // GPA0–5
const uint8_t mcp2LEDPins[NUM_BUTTONS]    = {8, 9, 10, 11, 12, 13};  // GPB0–5
bool lastButtonState2[NUM_BUTTONS];
unsigned long lastButton2DebounceTime[NUM_BUTTONS] = {0};

// Interrupt flags
volatile bool mcp1InterruptFlag = false;
volatile bool mcp2InterruptFlag = false;

// === INTERRUPT HANDLERS ===
void IRAM_ATTR onMCP1Interrupt() {
  mcp1InterruptFlag = true;
}
void IRAM_ATTR onMCP2Interrupt() {
  mcp2InterruptFlag = true;
}

// === CALLBACK FUNCTIONS ===
void onEncoderMoved(uint8_t id, int newPosition) {
  Serial.printf("Encoder %d moved to %d\n", id, newPosition);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Encoder");
  lcd.setCursor(0,1);
  lcd.print(id);
  lcd.setCursor(0,2);
  lcd.print("Moved to");
  lcd.setCursor(0,3);
  lcd.print(newPosition);
}

void onEncoderButtonPressed(uint8_t id) {
  Serial.printf("Encoder %d button pressed\n", id);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Encoder");
  lcd.setCursor(0,1);
  lcd.print(id);
  lcd.setCursor(0,2);
  lcd.print("Button Pressed");
}

void onMCP2ButtonPressed(uint8_t id) {
  Serial.printf("MCP2 Button %d pressed\n", id);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("MCP2 Button");
  lcd.setCursor(0,1);
  lcd.print(id);
  lcd.setCursor(0,2);
  lcd.print("Pressed");

  for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
    mcp2.digitalWrite(mcp2LEDPins[i], (i == id) ? HIGH : LOW);
  }
}

void setup() {
  Serial.begin(115200);

  lcd.init();                      // initialize the lcd 
  lcd.backlight();


  // === MCP1 SETUP ===
  if (!mcp1.begin_I2C(0x20)) {
    Serial.println("MCP1 not found!");
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
    Serial.println("MCP2 not found!");
    lcd.setCursor(0,1);
    lcd.print("MCP2 not found!");
    while (1);
  }

  for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
    mcp2.pinMode(mcp2ButtonPins[i], INPUT_PULLUP);
    mcp2.setupInterruptPin(mcp2ButtonPins[i], FALLING);
    mcp2.pinMode(mcp2LEDPins[i], OUTPUT);
    mcp2.digitalWrite(mcp2LEDPins[i], LOW);
    lastButtonState2[i] = mcp2.digitalRead(mcp2ButtonPins[i]);
  }

  pinMode(MCP2_INT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(MCP2_INT_PIN), onMCP2Interrupt, FALLING);

  Serial.println("Setup complete.");
  lcd.setCursor(0,3);
  lcd.print("Setup complete.");
}

void loop() {
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
