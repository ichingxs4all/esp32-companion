void setupWIFI(){
setLedBlue(2); //Display WIFI manager status on third led
WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP  
if(WMRESET) wm.resetSettings();
wm.addParameter(&custom_ipaddress);
//wm.addParameter(&custom_mqtt_server);

wm.setConfigPortalBlocking(false);
wm.setConfigPortalTimeout(60);
wm.setSaveParamsCallback(saveParamsCallback);
}

void connectWIFI(){
    setLedBlue(3); //Display WIFI status on fourth led
  //automatically connect using saved credentials if they exist
    //If connection fails it starts an access point with the specified name
    if(wm.autoConnect("AutoConnectAP")){
        if(debug)Serial.println("connected...yeey :)");
        //lcd.clear();
        lcd.setCursor(0,2);
        lcd.print("WiFi connected    ");
        delay(1000);
    }
    else {
        if(debug)Serial.println("Configportal running");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Configportal running!");
        delay(1000);
    }
}
