bool debug = true;
//////////////////////// Wifi Manager library/////////////////////////////////////////
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
WiFiManager wm;
WiFiManagerParameter custom_ipaddress("companion_ip", "companion IP", "", 15, "pattern='\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}'");  // custom input attrs (ip mask)

#define WMRESET false
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
#define USEOTA
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


//bool debug = true;

bool setIris = true;
bool setGain = false;
bool setWB = false;
bool setFocus = false;
bool setZoom = false;

int iris;
char irisString;

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
SimpleRotary encoder(25, 26, 27);
SimpleRotaryAdapter rotaryInput(&menu, &encoder);  // Rotary input adapter

Button enterBtn(33);
ButtonAdapter enterBtnA(&menu, &enterBtn, ENTER);
Button backBtn(32);
ButtonAdapter backBtnA(&menu, &backBtn, BACK);

OSCMessage msgPress("");
OSCMessage msgRotLeft("");
OSCMessage msgRotRight("");


//---Column 1 
//Row 0
OSCMessage msgC1_R0_press("/location/1/0/1/press");
OSCMessage msgC1_R0_rotLeft("/location/1/0/1/rotate-left");
OSCMessage msgC1_R0_rotRight("/location/1/0/1/rotate-right");

//Row 1
OSCMessage msgC1_R1_press("/location/1/1/1/press");
OSCMessage msgC1_R1_rotLeft("/location/1/1/1/rotate-left");
OSCMessage msgC1_R1_rotRight("/location/1/1/1/rotate-right");

//Row 2
OSCMessage msgC1_R2_press("/location/1/2/1/press");
OSCMessage msgC1_R2_rotLeft("/location/1/2/1/rotate-left");
OSCMessage msgC1_R2_rotRight("/location/1/2/1/rotate-right");

//Row 3
OSCMessage msgC1_R3_press("/location/1/3/1/press");
OSCMessage msgC1_R3_rotLeft("/location/1/3/1/rotate-left");
OSCMessage msgC1_R3_rotRight("/location/1/3/1/rotate-right");

//Row 4
OSCMessage msgC1_R4_press("/location/1/4/1/press");
OSCMessage msgC1_R4_rotLeft("/location/1/4/1/rotate-left");
OSCMessage msgC1_R4_rotRight("/location/1/4/1/rotate-right");

//Row 5
OSCMessage msgC1_R5_press("/location/1/5/1/press");
OSCMessage msgC1_R5_rotLeft("/location/1/5/1/rotate-left");
OSCMessage msgC1_R5_rotRight("/location/1/5/1/rotate-right");

//Row 6
OSCMessage msgC1_R6_press("/location/1/6/1/press");
OSCMessage msgC1_R6_rotLeft("/location/1/6/1/rotate-left");
OSCMessage msgC1_R6_rotRight("/location/1/6/1/rotate-right");

//Row 7
OSCMessage msgC1_R7_press("/location/1/7/1/press");
OSCMessage msgC1_R7_rotLeft("/location/1/7/1/rotate-left");
OSCMessage msgC1_R7_rotRight("/location/1/7/1/rotate-right");



//---Column 2 --------------------------------------------
//Row 0
OSCMessage msgC2_R0_press("/location/1/0/2/press");
OSCMessage msgC2_R0_rotLeft("/location/1/0/2/rotate-left");
OSCMessage msgC2_R0_rotRight("/location/1/0/2/rotate-right");

//Row 1
OSCMessage msgC2_R1_press("/location/1/1/2/press");
OSCMessage msgC2_R1_rotLeft("/location/1/1/2/rotate-left");
OSCMessage msgC2_R1_rotRight("/location/1/1/2/rotate-right");

//Row 2
OSCMessage msgC2_R2_press("/location/1/2/2/press");
OSCMessage msgC2_R2_rotLeft("/location/1/2/2/rotate-left");
OSCMessage msgC2_R2_rotRight("/location/1/2/2/rotate-right");

//Row 3
OSCMessage msgC2_R3_press("/location/1/3/2/press");
OSCMessage msgC2_R3_rotLeft("/location/1/3/2/rotate-left");
OSCMessage msgC2_R3_rotRight("/location/1/3/2/rotate-right");

//Row 4
OSCMessage msgC2_R4_press("/location/1/4/2/press");
OSCMessage msgC2_R4_rotLeft("/location/1/4/2/rotate-left");
OSCMessage msgC2_R4_rotRight("/location/1/4/2/rotate-right");

//Row 5
OSCMessage msgC2_R5_press("/location/1/5/2/press");
OSCMessage msgC2_R5_rotLeft("/location/1/5/2/rotate-left");
OSCMessage msgC2_R5_rotRight("/location/1/5/2/rotate-right");

//Row 6
OSCMessage msgC2_R6_press("/location/1/6/2/press");
OSCMessage msgC2_R6_rotLeft("/location/1/6/2/rotate-left");
OSCMessage msgC2_R6_rotRight("/location/1/6/2/rotate-right");

//Row 7
OSCMessage msgC2_R7_press("/location/1/7/2/press");
OSCMessage msgC2_R7_rotLeft("/location/1/7/2/rotate-left");
OSCMessage msgC2_R7_rotRight("/location/1/7/2/rotate-right");

//---Column 3 -------------------------------------------
//Row 0
OSCMessage msgC3_R0_press("/location/1/0/3/press");
OSCMessage msgC3_R0_rotLeft("/location/1/0/3/rotate-left");
OSCMessage msgC3_R0_rotRight("/location/1/0/3/rotate-right");

//Row 1
OSCMessage msgC3_R1_press("/location/1/1/3/press");
OSCMessage msgC3_R1_rotLeft("/location/1/1/3/rotate-left");
OSCMessage msgC3_R1_rotRight("/location/1/1/3/rotate-right");

//Row 2
OSCMessage msgC3_R2_press("/location/1/2/3/press");
OSCMessage msgC3_R2_rotLeft("/location/1/2/3/rotate-left");
OSCMessage msgC3_R2_rotRight("/location/1/2/3/rotate-right");

//Row 3
OSCMessage msgC3_R3_press("/location/1/3/3/press");
OSCMessage msgC3_R3_rotLeft("/location/1/3/3/rotate-left");
OSCMessage msgC3_R3_rotRight("/location/1/3/3/rotate-right");

//Row 4
OSCMessage msgC3_R4_press("/location/1/4/3/press");
OSCMessage msgC3_R4_rotLeft("/location/1/4/3/rotate-left");
OSCMessage msgC3_R4_rotRight("/location/1/4/3/rotate-right");

//Row 5
OSCMessage msgC3_R5_press("/location/1/5/3/press");
OSCMessage msgC3_R5_rotLeft("/location/1/5/3/rotate-left");
OSCMessage msgC3_R5_rotRight("/location/1/5/3/rotate-right");

//Row 6
OSCMessage msgC3_R6_press("/location/1/6/3/press");
OSCMessage msgC3_R6_rotLeft("/location/1/6/3/rotate-left");
OSCMessage msgC3_R6_rotRight("/location/1/6/3/rotate-right");

//Row 7
OSCMessage msgC3_R7_press("/location/1/7/3/press");
OSCMessage msgC3_R7_rotLeft("/location/1/7/3/rotate-left");
OSCMessage msgC3_R7_rotRight("/location/1/7/3/rotate-right");


//---Column 4 -------------------------------------------
//Row 0
OSCMessage msgC4_R0_press("/location/1/0/4/press");
OSCMessage msgC4_R0_rotLeft("/location/1/0/4/rotate-left");
OSCMessage msgC4_R0_rotRight("/location/1/0/4/rotate-right");

//Row 1
OSCMessage msgC4_R1_press("/location/1/1/4/press");
OSCMessage msgC4_R1_rotLeft("/location/1/1/4/rotate-left");
OSCMessage msgC4_R1_rotRight("/location/1/1/4/rotate-right");

//Row 2
OSCMessage msgC4_R2_press("/location/1/2/4/press");
OSCMessage msgC4_R2_rotLeft("/location/1/2/4/rotate-left");
OSCMessage msgC4_R2_rotRight("/location/1/2/4/rotate-right");

//Row 3
OSCMessage msgC4_R3_press("/location/1/3/4/press");
OSCMessage msgC4_R3_rotLeft("/location/1/3/4/rotate-left");
OSCMessage msgC4_R3_rotRight("/location/1/3/4/rotate-right");

//Row 4
OSCMessage msgC4_R4_press("/location/1/4/4/press");
OSCMessage msgC4_R4_rotLeft("/location/1/4/4/rotate-left");
OSCMessage msgC4_R4_rotRight("/location/1/4/4/rotate-right");

//Row 5
OSCMessage msgC4_R5_press("/location/1/5/4/press");
OSCMessage msgC4_R5_rotLeft("/location/1/5/4/rotate-left");
OSCMessage msgC4_R5_rotRight("/location/1/5/4/rotate-right");

//Row 6
OSCMessage msgC4_R6_press("/location/1/6/4/press");
OSCMessage msgC4_R6_rotLeft("/location/1/6/4/rotate-left");
OSCMessage msgC4_R6_rotRight("/location/1/6/4/rotate-right");

//Row 7
OSCMessage msgC4_R7_press("/location/1/7/4/press");
OSCMessage msgC4_R7_rotLeft("/location/1/7/4/rotate-left");
OSCMessage msgC4_R7_rotRight("/location/1/7/4/rotate-right");

//---Column 5 -------------------------------------------
//Row 0
OSCMessage msgC5_R0_press("/location/1/0/5/press");
OSCMessage msgC5_R0_rotLeft("/location/1/0/5/rotate-left");
OSCMessage msgC5_R0_rotRight("/location/1/0/5/rotate-right");

//Row 1
OSCMessage msgC5_R1_press("/location/1/1/5/press");
OSCMessage msgC5_R1_rotLeft("/location/1/1/5/rotate-left");
OSCMessage msgC5_R1_rotRight("/location/1/1/5/rotate-right");

//Row 2
OSCMessage msgC5_R2_press("/location/1/2/5/press");
OSCMessage msgC5_R2_rotLeft("/location/1/2/5/rotate-left");
OSCMessage msgC5_R2_rotRight("/location/1/2/5/rotate-right");

//Row 3
OSCMessage msgC5_R3_press("/location/1/3/5/press");
OSCMessage msgC5_R3_rotLeft("/location/1/3/5/rotate-left");
OSCMessage msgC5_R3_rotRight("/location/1/3/5/rotate-right");

//Row 4
OSCMessage msgC5_R4_press("/location/1/4/5/press");
OSCMessage msgC5_R4_rotLeft("/location/1/4/5/rotate-left");
OSCMessage msgC5_R4_rotRight("/location/1/4/5/rotate-right");

//Row 5
OSCMessage msgC5_R5_press("/location/1/5/5/press");
OSCMessage msgC5_R5_rotLeft("/location/1/5/5/rotate-left");
OSCMessage msgC5_R5_rotRight("/location/1/5/5/rotate-right");

//Row 6
OSCMessage msgC5_R6_press("/location/1/6/5/press");
OSCMessage msgC5_R6_rotLeft("/location/1/6/5/rotate-left");
OSCMessage msgC5_R6_rotRight("/location/1/6/5/rotate-right");

//Row 7
OSCMessage msgC5_R7_press("/location/1/7/5/press");
OSCMessage msgC5_R7_rotLeft("/location/1/7/5/rotate-left");
OSCMessage msgC5_R7_rotRight("/location/1/7/5/rotate-right");

//---Column 6 -------------------------------------------
//Row 0
OSCMessage msgC6_R0_press("/location/1/0/6/press");
OSCMessage msgC6_R0_rotLeft("/location/1/0/6/rotate-left");
OSCMessage msgC6_R0_rotRight("/location/1/0/6/rotate-right");

//Row 1
OSCMessage msgC6_R1_press("/location/1/1/6/press");
OSCMessage msgC6_R1_rotLeft("/location/1/1/6/rotate-left");
OSCMessage msgC6_R1_rotRight("/location/1/1/6/rotate-right");

//Row 2
OSCMessage msgC6_R2_press("/location/1/2/6/press");
OSCMessage msgC6_R2_rotLeft("/location/1/2/6/rotate-left");
OSCMessage msgC6_R2_rotRight("/location/1/2/6/rotate-right");

//Row 3
OSCMessage msgC6_R3_press("/location/1/3/6/press");
OSCMessage msgC6_R3_rotLeft("/location/1/3/6/rotate-left");
OSCMessage msgC6_R3_rotRight("/location/1/3/6/rotate-right");

//Row 4
OSCMessage msgC6_R4_press("/location/1/4/6/press");
OSCMessage msgC6_R4_rotLeft("/location/1/4/6/rotate-left");
OSCMessage msgC6_R4_rotRight("/location/1/4/6/rotate-right");

//Row 5
OSCMessage msgC6_R5_press("/location/1/5/6/press");
OSCMessage msgC6_R5_rotLeft("/location/1/5/6/rotate-left");
OSCMessage msgC6_R5_rotRight("/location/1/5/6/rotate-right");

//Row 6
OSCMessage msgC6_R6_press("/location/1/6/6/press");
OSCMessage msgC6_R6_rotLeft("/location/1/6/6/rotate-left");
OSCMessage msgC6_R6_rotRight("/location/1/6/6/rotate-right");

//Row 7
OSCMessage msgC6_R7_press("/location/1/7/6/press");
OSCMessage msgC6_R7_rotLeft("/location/1/7/6/rotate-left");
OSCMessage msgC6_R7_rotRight("/location/1/7/6/rotate-right");

void setup() {
setupLeds();
setupLCD();
setupWIFI();
setupEncoders_Buttons();


if(debug){
 Serial.begin(115200);
 Serial.println("Setup complete.");
 Serial.flush();
}

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

  #ifdef USEOTA
    ArduinoOTA.begin();
  #endif

setupMenu(); 
menu.setScreen(mainScreen);
clearAllLeds();
}

void loop() {

   #ifdef USEOTA
  ArduinoOTA.handle();
  #endif
  
  wm.process();
  enterBtnA.observe();
  backBtnA.observe();
  rotaryInput.observe();
  checkEncoders_Buttons();
  checkOSC_Receive();
}

void saveParamsCallback() {
  if(debug){
  Serial.println("Get Params:");
  Serial.print(custom_ipaddress.getID());
  Serial.print(" : ");
  Serial.println(custom_ipaddress.getValue());
  }
}

