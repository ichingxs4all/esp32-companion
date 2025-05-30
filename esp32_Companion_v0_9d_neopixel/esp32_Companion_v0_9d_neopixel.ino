bool debug = true;
//////////////////////// Wifi Manager library/////////////////////////////////////////
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

WiFiManager wm;
WiFiManagerParameter custom_ipaddress("companion_ip", "companion IP", "", 15, "pattern='\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}'");  // custom input attrs (ip mask)
WiFiManagerParameter custom_mqtt_server("server", "mqtt server", "", 40);

bool WMRESET = false;
/////////////////////////// OSC library CNMAT //////////////////////////////////////

#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>

WiFiUDP Udp;  // A UDP instance to let us send and receive packets over UDP

OSCErrorCode error;

IPAddress outIp(192, 168, 188, 32);  // remote IP of the receiving computer


const unsigned int outPort = 12321;       // remote port to send OSC messages to
const unsigned int localPort = 8888;  // local port to listen for OSC packets (actually not used for sending)

/////////////////////Arduino OTA library /////////////////////////////////////////////
#include <ESPmDNS.h>
#include <NetworkUdp.h>
#include <ArduinoOTA.h>

/////////////////////LCDMenu library /////////////////////////////////////////////
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
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

#define LCD_ENTER_BTN 33
#define LCD_BACK_BTN 32


/////////////////////////Adadfruit MCP23x17 library ////////////////////////////////////////
#include <Adafruit_MCP23X17.h>

Adafruit_MCP23X17 mcp1; // Encoders
Adafruit_MCP23X17 mcp2; // Buttons + LEDs


 /////////////////////////Adafruit Neopixel library ////////////////////////////////////////
#include <Adafruit_NeoPixel.h>

// How many leds in your strip?
#define NUMPIXELS 6

// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define PIN 19

// Define the array of leds
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


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
//const uint8_t mcp2LEDPins[NUM_BUTTONS] = {0, 1, 2, 3, 4, 5,6,7};      // GPB0–5
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


bool setIris = true;
bool setGain = false;
bool setWB = false;
bool setFocus = false;
bool setZoom = false;


// Declare the callbacks
void toggleBacklight(bool isOn);
void inputCallback(char* value);

std::vector<const char*> cameras = {"1", "2", "3", "4", "5", "6"};
std::vector<const char*> modus = {"Iris", "Gain", "WB","Focus","Zoom"};

int cam = 0;
int mode = 0; 
int page = 1;

// clang-format off
MENU_SCREEN(mainScreen, mainItems,
    ITEM_LIST("Modus", modus, [](const uint8_t mod) {      
      if (mod == 0 )toggleIris();
      if (mod == 1 )toggleGain();
      if (mod == 2 )toggleWB();
      if (mod == 3 )toggleFocus();
      if (mod == 4 )toggleZoom();
      }));
    //ITEM_TOGGLE("Backlight", toggleBacklight));
// clang-format on

LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);
LiquidCrystal_I2CAdapter lcdAdapter(&lcd);
CharacterDisplayRenderer renderer(&lcdAdapter, LCD_COLS, LCD_ROWS);
LcdMenu menu(renderer);
SimpleRotary encoder(LCD_ENCA, LCD_ENCB, LCD_ENCSW);
SimpleRotaryAdapter rotaryInput(&menu, &encoder);  // Rotary input adapter

Button enterBtn(LCD_ENTER_BTN);
ButtonAdapter enterBtnA(&menu, &enterBtn, ENTER);
Button backBtn(LCD_BACK_BTN);
ButtonAdapter backBtnA(&menu, &backBtn, BACK);

#include "osc_messages.h"

void setup() {
  if(debug){
 Serial.begin(115200);
 Serial.flush();
}

checkResetConfig();
setupLeds();
setupLCD();
setupWIFI();
setupEncoders_Buttons();
connectWIFI();

delay(2000);

if(debug)Serial.println("Starting UDP");
Udp.begin(localPort);
if(debug)Serial.print("Local port: ");

#ifdef ESP32
  if(debug)Serial.println(localPort);
#else
  if(debug)Serial.println(Udp.localPort());
#endif

lcd.setCursor(0,2);
lcd.print("OSC UDP started on");

if(debug){
  Serial.print("Companion IP: ");
  Serial.println(outIp);
}

lcd.setCursor(0,3);
lcd.print(outIp);
lcd.print(" ");
lcd.print(outPort);

delay(2000);

setupMenu(); 
menu.setScreen(mainScreen);
clearAllLeds();
}

void loop() {
  wm.process();
  enterBtnA.observe();
  backBtnA.observe();
  rotaryInput.observe();
  checkEncoders_Buttons();
  checkOSC_Receive();
  ArduinoOTA.handle();
}

void saveParamsCallback() {
  if(debug){
  Serial.println("Get Params:");
  Serial.print(custom_ipaddress.getID());
  Serial.print(" : ");
  Serial.println(custom_ipaddress.getValue());
  Serial.print(custom_mqtt_server.getID());
  Serial.print(" : ");
  Serial.println(custom_mqtt_server.getValue());
  }
}

void checkResetConfig(){
  pinMode(LCD_ENTER_BTN , INPUT_PULLUP);
  if(digitalRead(LCD_ENTER_BTN)== 0) WMRESET = true;
  Serial.println("Reset Wifi settings");
}
