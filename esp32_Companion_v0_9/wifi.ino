void setupWIFI(){
WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP  
//wm.resetSettings();
wm.addParameter(&custom_ipaddress);
wm.setConfigPortalBlocking(false);
wm.setConfigPortalTimeout(60);
wm.setSaveParamsCallback(saveParamsCallback);
}

void connectWIFI(){
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
}
