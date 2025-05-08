#include <Wire.h>
#include <Adafruit_MCP23X17.h>
#include <LiquidCrystal_I2C.h>

#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
WiFiManager wm;

#include <Button.h>
#include <ItemInputCharset.h>
#include <ItemList.h>
#include <ItemToggle.h>
#include <LcdMenu.h>
#include <MenuScreen.h>
#include <SimpleRotary.h>
#include <display/LiquidCrystal_I2CAdapter.h>
#include <input/ButtonAdapter.h>
#include <input/SimpleRotaryAdapter.h>
#include <renderer/CharacterDisplayRenderer.h>

#define LCD_ROWS 4
#define LCD_COLS 20
#define LCD_ENCA 25
#define LCD_ENCB 26
#define LCD_ENCSW 27

//LiquidCrystal_I2C lcd(0x27,LCD_COLS,LCD_ROWS);  // set the LCD address to 0x27 for a 20chars and 4 line display

Adafruit_MCP23X17 mcp1; // Encoders
Adafruit_MCP23X17 mcp2; // Buttons + LEDs

// === CONFIGURATION ===
const uint8_t MCP1_INT_PIN = 34; // INT from mcp1
const uint8_t MCP2_INT_PIN = 35; // INT from mcp2
const unsigned long DEBOUNCE_DELAY_MS = 10;

// === MCP1: Encoders ===
const uint8_t NUM_ENCODERS = 5;
const uint8_t encoderPins[NUM_ENCODERS][3] = {
  {1, 0, 2},   // A, B, Button
  {4, 3, 5},
  {7, 6, 8},
  {10, 9, 11},
  {13, 12, 14}
};



int encoderPositions[NUM_ENCODERS] = {0};
bool lastAState[NUM_ENCODERS];
bool lastButtonState[NUM_ENCODERS];
unsigned long lastADebounceTime[NUM_ENCODERS] = {0};
unsigned long lastBtnDebounceTime[NUM_ENCODERS] = {0};

// === MCP2: Buttons and LEDs ===
const uint8_t NUM_BUTTONS = 8;
const uint8_t mcp2LEDPins[NUM_BUTTONS] = {0, 1, 2, 3, 4, 5,6,7};      // GPB0–5
const uint8_t mcp2ButtonPins[NUM_BUTTONS] = {8, 9, 10, 11, 12, 13, 14, 15};  // GPB0–5

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


bool debug = true;


// Declare the callbacks
void toggleBacklight(bool isOn);
void inputCallback(char* value);

std::vector<const char*> cameras = {"1", "2", "3", "4", "5", "6"};

// clang-format off
MENU_SCREEN(mainScreen, mainItems,
    //ITEM_INPUT_CHARSET("User", (const char*)"ABCDEFGHIJKLMNOPQRSTUVWXYZ", inputCallback),
    ITEM_LIST("Camera", cameras, [](const uint8_t camera) { Serial.println(cameras[camera]); }),
    ITEM_BASIC("GAIN"),
    ITEM_BASIC("IRIS"),
    ITEM_BASIC("WB"),
    ITEM_TOGGLE("Backlight", toggleBacklight));
// clang-format on

LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);
LiquidCrystal_I2CAdapter lcdAdapter(&lcd);
CharacterDisplayRenderer renderer(&lcdAdapter, LCD_COLS, LCD_ROWS);
LcdMenu menu(renderer);
SimpleRotary encoder(25, 26, 27);
SimpleRotaryAdapter rotaryInput(&menu, &encoder);  // Rotary input adapter

Button upBtn(33);
ButtonAdapter upBtnA(&menu, &upBtn, UP);
Button downBtn(32);
ButtonAdapter downBtnA(&menu, &downBtn, DOWN);



void setup() {
lcd.init();                      // initialize the lcd 
lcd.backlight();
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Camera Shader");
lcd.setCursor(0,2);
lcd.print("Setting up WiFi");

WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP  


upBtn.begin();
downBtn.begin();
renderer.begin();

  setupEncoders_Buttons();

  if(debug)Serial.begin(115200);
  if(debug)Serial.println("Setup complete.");

  wm.setConfigPortalBlocking(false);
    wm.setConfigPortalTimeout(60);
    //automatically connect using saved credentials if they exist
    //If connection fails it starts an access point with the specified name
    if(wm.autoConnect("AutoConnectAP")){
        if(debug)Serial.println("connected...yeey :)");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("WiFi connected !");
        delay(1000);
    }
    else {
        if(debug)Serial.println("Configportal running");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Configportal running!");
        delay(1000);
    }

  //reset settings - wipe credentials for testing
    //wm.resetSettings();

  menu.setScreen(mainScreen);
 
}

void loop() {
   wm.process();
  upBtnA.observe();
  downBtnA.observe();
  rotaryInput.observe();
  checkEncoders_Buttons();
}

// Define the callbacks
void toggleBacklight(bool isOn) {
    lcdAdapter.setBacklight(isOn);
}

void inputCallback(char* value) {
    Serial.println(value);
}