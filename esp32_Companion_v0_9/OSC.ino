void checkOSC_Receive(){
   OSCMessage rcvmsg;
  int size = Udp.parsePacket();
  if (size > 0) {
    while (size--) {
      rcvmsg.fill(Udp.read());
    }
    if (!rcvmsg.hasError()) {
      rcvmsg.dispatch("/camera/1", receive_C1);
      rcvmsg.dispatch("/camera/2", receive_C2);
      rcvmsg.dispatch("/camera/3", receive_C3);
      rcvmsg.dispatch("/camera/4", receive_C4);
      rcvmsg.dispatch("/camera/5", receive_C5);
      rcvmsg.dispatch("/camera/6", receive_C6);
    } else {
      error = rcvmsg.getError();
      if(debug)Serial.print("error: ");
      if(debug)Serial.println(error);
    }
  }
}

void sendOSC_Press(){
    Udp.beginPacket(outIp, outPort);
    msgPress.send(Udp);
    Udp.endPacket();
    msgPress.empty();  
}

void sendOSC_RotLeft(){
    Udp.beginPacket(outIp, outPort);
    msgRotLeft.send(Udp);
    Udp.endPacket();
    msgRotLeft.empty();  
}

void sendOSC_RotRight(){
    Udp.beginPacket(outIp, outPort);
    msgRotRight.send(Udp);
    Udp.endPacket();
    msgRotRight.empty();  
}

void receive_C1(OSCMessage &rcvmsg) {
  lcd.setCursor(0,0);
  lcd.print("Camera 1");
  lcd.setCursor(0, 3);

  if(rcvmsg.isString(0))
    {
         int length=rcvmsg.getDataLength(0);
         if(length<16)
         {
           char str[length];
           rcvmsg.getString(0,str,length);
           if(debug)Serial.print(str);
           lcd.setCursor(0, 3);
           lcd.print(str);
           lcd.print("    ");
         }
    }
    if(debug)Serial.println();
}



void receive_C2(OSCMessage &rcvmsg) {
  lcd.setCursor(0,0);
  lcd.print("Camera 2");
  lcd.setCursor(0, 3);

  if(rcvmsg.isString(0))
    {
         int length=rcvmsg.getDataLength(0);
         if(length<16)
         {
           char str[length];
           rcvmsg.getString(0,str,length);
           if(debug)Serial.print(str);
           lcd.setCursor(0, 3);
           lcd.print(str);
           lcd.print("    ");
         }
    }
    if(debug)Serial.println();
}

void receive_C3(OSCMessage &rcvmsg) {
  lcd.setCursor(0,0);
  lcd.print("Camera 3");
  lcd.setCursor(0, 3);

  if(rcvmsg.isString(0))
    {
         int length=rcvmsg.getDataLength(0);
         if(length<16)
         {
           char str[length];
           rcvmsg.getString(0,str,length);
           if(debug)Serial.print(str);
           lcd.setCursor(0, 3);
           lcd.print(str);
           lcd.print("    ");
         }
    }
    if(debug)Serial.println();
}

void receive_C4(OSCMessage &rcvmsg) {
  lcd.setCursor(0,0);
  lcd.print("Camera 4");
  lcd.setCursor(0, 3);

  if(rcvmsg.isString(0))
    {
         int length=rcvmsg.getDataLength(0);
         if(length<16)
         {
           char str[length];
           rcvmsg.getString(0,str,length);
           if(debug)Serial.print(str);
           lcd.setCursor(0, 3);
           lcd.print(str);
           lcd.print("    ");
         }
    }
    if(debug)Serial.println();
}

void receive_C5(OSCMessage &rcvmsg) {
  lcd.setCursor(0,0);
  lcd.print("Camera 5");
  lcd.setCursor(0, 3);

  if(rcvmsg.isString(0))
    {
         int length=rcvmsg.getDataLength(0);
         if(length<16)
         {
           char str[length];
           rcvmsg.getString(0,str,length);
           if(debug)Serial.print(str);
           lcd.setCursor(0, 3);
           lcd.print(str);
           lcd.print("    ");
         }
    }
    if(debug)Serial.println();
}

void receive_C6(OSCMessage &rcvmsg) {
  lcd.setCursor(0,0);
  lcd.print("Camera 6");
  lcd.setCursor(0, 3);

  if(rcvmsg.isString(0))
    {
         int length=rcvmsg.getDataLength(0);
         if(length<16)
         {
           char str[length];
           rcvmsg.getString(0,str,length);
           if(debug)Serial.print(str);
           lcd.setCursor(0, 3);
           lcd.print(str);
           lcd.print("    ");
         }
    }
    if(debug)Serial.println();
}