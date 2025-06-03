
#include <Preferences.h>
Preferences preferences;

#include <WiFiManager.h>  // https://github.com/tzapu/WiFiManager
WiFiManager wm;
WiFiManagerParameter custom_server("server", "server", "", 40);

IPAddress _ip;
uint32_t ip_addr;

char server[40];

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.mode(WIFI_STA);  // explicitly set mode, esp defaults to STA+AP
  // put your setup code here, to run once:


  //reset settings - wipe credentials for testing
  //wm.resetSettings();
  wm.addParameter(&custom_server);
  wm.setConfigPortalBlocking(false);
  wm.setSaveParamsCallback(saveParamsCallback);

  //automatically connect using saved credentials if they exist
  //If connection fails it starts an access point with the specified name
  if (wm.autoConnect("AutoConnectAP")) {
    Serial.println("connected...yeey :)");

  } else {
    Serial.println("Configportal running");
  }
  delay(5000);
  setServerIp();
}

void loop() {
  wm.process();
  // put your main code here, to run repeatedly:
}

void saveParamsCallback() {
  preferences.begin("server_prefs", false);
  Serial.println("Get Params:");
  Serial.print(custom_server.getID());

  Serial.print(" : ");
  strcpy(server, custom_server.getValue());
  IPAddress _ip;
  _ip.fromString(server);
  ip_addr = (uint32_t)_ip;
  Serial.println(ip_addr);
  preferences.putUInt("server_ip", ip_addr);
  preferences.end();
}


void setServerIp() {
  preferences.begin("server_prefs", false);
  ip_addr = preferences.getUInt("server_ip", 549234880);
  _ip = ip_addr;
  Serial.println(String("IP address is : ") + _ip[0] + String(". ") + _ip[1] + String(". ") + _ip[2] + String(". ") + _ip[3]);
  preferences.end();
}