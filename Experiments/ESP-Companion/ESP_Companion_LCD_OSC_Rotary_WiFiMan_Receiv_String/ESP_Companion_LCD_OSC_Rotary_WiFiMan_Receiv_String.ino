/*---------------------------------------------------------------------------------------------

  Open Sound Control (OSC) library for the ESP8266/ESP32

  Example for sending messages from the ESP8266/ESP32 to a remote computer
  The example is sending "hello, osc." to the address "/test".

  This example code is in the public domain.

--------------------------------------------------------------------------------------------- */
#include <WiFiManager.h>  // https://github.com/tzapu/WiFiManager
WiFiManager wm;
//WiFiManagerParameter custom_mqtt_server("server", "mqtt server", "", 40);
WiFiManagerParameter custom_ipaddress("companion_ip", "companion IP", "", 15, "pattern='\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}'");  // custom input attrs (ip mask)
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>
  /////////////////////////////////////////////////////////////////

#include "Button2.h"    //  https://github.com/LennartHennigs/Button2
#include "ESPRotary.h"  //ESPRotary

ESPRotary r;
Button2 b;

/////////////////////////////////////////////////////////////////

#define ROTARY_PIN1 D5
#define ROTARY_PIN2 D6
#define BUTTON_PIN D4

#define CLICKS_PER_STEP 8  // this number depends on your rotary encoder

// include the library code:
#include <Arduino.h>
#include <LiquidCrystalIO.h>

// When using the I2C version, these two extra includes are always needed. Doing this reduces the memory slightly for
// users that are not using I2C.
#include <IoAbstractionWire.h>
#include <Wire.h>

// For most standard I2C backpacks one of the two helper functions below will create you a liquid crystal instance
// that's ready configured for I2C. Important Note: this method assumes a PCF8574 running at 100Khz. If otherwise
// use a custom configuration as you see in many other examples.

// If your backpack is wired RS,RW,EN then use this version
LiquidCrystalI2C_RS_EN(lcd, 0x27, false)

  // If your backpack is wired EN,RW,RS then use this version instead of the above.
  //LiquidCrystalI2C_EN_RS(lcd, 0x27, false)



WiFiUDP Udp;  // A UDP instance to let us send and receive packets over UDP

OSCErrorCode error;

const IPAddress outIp(192, 168, 188, 166);  // remote IP of your computer

const unsigned int outPort = 12321;       // remote port to receive OSC

//const unsigned int outPort = 8000;

const unsigned int localPort = 8888;  // local port to listen for OSC packets (actually not used for sending)

bool buttonPress = false;

int positionA, positionB, positionC;
char direction;
bool debug = true;

int iris;
char irisString;

void setup() {
  WiFi.mode(WIFI_STA);  // explicitly set mode, esp defaults to STA+AP

  //wm.resetSettings();
  wm.addParameter(&custom_ipaddress);
  //wm.addParameter(&custom_mqtt_server);
  wm.setConfigPortalBlocking(false);
  wm.setSaveParamsCallback(saveParamsCallback);

  if (debug) {
    Serial.begin(115200);
    Serial.println("ESP8266 Companion Starting");
  }
  Wire.begin();
  setupLCD();

  delay(5000);

  if (debug) Serial.flush();


  //automatically connect using saved credentials if they exist
  //If connection fails it starts an access point with the specified name
  if (wm.autoConnect("AutoConnectAP")) {
    
    if(debug)Serial.println("Connected");

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(WiFi.localIP());
    lcd.setCursor(0, 1);
    lcd.print(outIp);

  } else {
    if(debug) Serial.println("Configportal running");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Configportal start");
  }

  delay(2000);
  if(debug)Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
#ifdef ESP32
  Serial.println(localPort);
#else
  if(debug)Serial.println(Udp.localPort());
#endif

  setupButtonRotary();
  delay(500);
}

void loop() {


  wm.process();
  r.loop();
  b.loop();

  if (buttonPress) {
    sendOSCA();
    buttonPress = false;
  }

  OSCMessage rcvmsg;
  int size = Udp.parsePacket();
  if (size > 0) {
    while (size--) {
      rcvmsg.fill(Udp.read());
    }
    if (!rcvmsg.hasError()) {
      rcvmsg.dispatch("/camera1/irispos", receiveIris);
    } else {
      error = rcvmsg.getError();
      if(debug)Serial.print("error: ");
      if(debug)Serial.println(error);
    }
  }
}

void saveParamsCallback() {
  /*
  Serial.println("Get Params:");
  Serial.print(custom_mqtt_server.getID());
  Serial.print(" : ");
  Serial.println(custom_mqtt_server.getValue());
  */
}

void receiveIris(OSCMessage &rcvmsg) {
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Iris Camera 1");
lcd.setCursor(0,1);

  if(rcvmsg.isString(0))
    {
         int length=rcvmsg.getDataLength(0);
         if(length<16)
         {
           char str[length];
           rcvmsg.getString(0,str,length);
           if(debug)Serial.print(str);
           lcd.print(str);
         }
    }
    if(debug)Serial.println();
}
