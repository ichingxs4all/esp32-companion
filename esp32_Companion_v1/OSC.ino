void checkOSC_Receive() {
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

      rcvmsg.dispatch("/tally/1/PRG", receive_C1_tally_PRG);
      rcvmsg.dispatch("/tally/2/PRG", receive_C2_tally_PRG);
      rcvmsg.dispatch("/tally/3/PRG", receive_C3_tally_PRG);
      rcvmsg.dispatch("/tally/4/PRG", receive_C4_tally_PRG);
      rcvmsg.dispatch("/tally/5/PRG", receive_C5_tally_PRG);
      rcvmsg.dispatch("/tally/6/PRG", receive_C6_tally_PRG);

      rcvmsg.dispatch("/tally/1/PRV", receive_C1_tally_PRV);
      rcvmsg.dispatch("/tally/2/PRV", receive_C2_tally_PRV);
      rcvmsg.dispatch("/tally/3/PRV", receive_C3_tally_PRV);
      rcvmsg.dispatch("/tally/4/PRV", receive_C4_tally_PRV);
      rcvmsg.dispatch("/tally/5/PRV", receive_C5_tally_PRV);
      rcvmsg.dispatch("/tally/6/PRV", receive_C6_tally_PRV);

      rcvmsg.dispatch("/reset", reset_system);
      rcvmsg.dispatch("/wifireset", wifi_reset);

    } else {
      error = rcvmsg.getError();
      if (debug) Serial.print("error: ");
      if (debug) Serial.println(error);
    }
  }
}

void sendOSC_Press() {
  Udp.beginPacket(outIp, outPort);
  msgPress.send(Udp);
  Udp.endPacket();
  msgPress.empty();
}

void sendOSC_RotLeft() {
  Udp.beginPacket(outIp, outPort);
  msgRotLeft.send(Udp);
  Udp.endPacket();
  msgRotLeft.empty();
}

void sendOSC_RotRight() {
  Udp.beginPacket(outIp, outPort);
  msgRotRight.send(Udp);
  Udp.endPacket();
  msgRotRight.empty();
}

void receive_C1(OSCMessage &rcvmsg) {
  lcd.setCursor(0, 1);
  lcd.print("Camera 1");
  lcd.setCursor(0, 3);

  if (rcvmsg.isString(0)) {
    int length = rcvmsg.getDataLength(0);
    if (length < 16) {
      char str[length];
      rcvmsg.getString(0, str, length);
      if (debug) Serial.print(str);
      lcd.setCursor(0, 3);
      lcd.print(str);
      lcd.print("          ");
    }
  }
  if (debug) Serial.println();
}



void receive_C2(OSCMessage &rcvmsg) {
  lcd.setCursor(0, 1);
  lcd.print("Camera 2");
  lcd.setCursor(0, 3);

  if (rcvmsg.isString(0)) {
    int length = rcvmsg.getDataLength(0);
    if (length < 16) {
      char str[length];
      rcvmsg.getString(0, str, length);
      if (debug) Serial.print(str);
      lcd.setCursor(0, 3);
      lcd.print(str);
      lcd.print("         ");
    }
  }
  if (debug) Serial.println();
}

void receive_C3(OSCMessage &rcvmsg) {
  lcd.setCursor(0, 1);
  lcd.print("Camera 3");
  lcd.setCursor(0, 3);

  if (rcvmsg.isString(0)) {
    int length = rcvmsg.getDataLength(0);
    if (length < 16) {
      char str[length];
      rcvmsg.getString(0, str, length);
      if (debug) Serial.print(str);
      lcd.setCursor(0, 3);
      lcd.print(str);
      lcd.print("         ");
    }
  }
  if (debug) Serial.println();
}

void receive_C4(OSCMessage &rcvmsg) {
  lcd.setCursor(0, 1);
  lcd.print("Camera 4");
  lcd.setCursor(0, 3);

  if (rcvmsg.isString(0)) {
    int length = rcvmsg.getDataLength(0);
    if (length < 16) {
      char str[length];
      rcvmsg.getString(0, str, length);
      if (debug) Serial.print(str);
      lcd.setCursor(0, 3);
      lcd.print(str);
      lcd.print("         ");
    }
  }
  if (debug) Serial.println();
}

void receive_C5(OSCMessage &rcvmsg) {
  lcd.setCursor(0, 1);
  lcd.print("Camera 5");
  lcd.setCursor(0, 3);

  if (rcvmsg.isString(0)) {
    int length = rcvmsg.getDataLength(0);
    if (length < 16) {
      char str[length];
      rcvmsg.getString(0, str, length);
      if (debug) Serial.print(str);
      lcd.setCursor(0, 3);
      lcd.print(str);
      lcd.print("         ");
    }
  }
  if (debug) Serial.println();
}

void receive_C6(OSCMessage &rcvmsg) {
  lcd.setCursor(0, 1);
  lcd.print("Camera 6");
  lcd.setCursor(0, 3);

  if (rcvmsg.isString(0)) {
    int length = rcvmsg.getDataLength(0);
    if (length < 16) {
      char str[length];
      rcvmsg.getString(0, str, length);
      if (debug) Serial.print(str);
      lcd.setCursor(0, 3);
      lcd.print(str);
      lcd.print("         ");
    }
  }
  if (debug) Serial.println();
}

//////////////////// Tally PRG //////////////////////

void receive_C1_tally_PRG(OSCMessage &rcvmsg) {
  if (rcvmsg.isString(0)) {
    int length = rcvmsg.getDataLength(0);
    if (length < 16) {
      char str[length];
      rcvmsg.getString(0, str, length);
     
      if (length == 4) {
        setLedBlack(0);
        if (debug) Serial.println("Camera 1 tally OFF");
      }

      if (length == 5) {
        setLedRed(0);
        if (debug) Serial.println("Camera 1 tally PROG");
      }

      if (length == 6) {
        setLedBlack(0);
        if (debug) Serial.println("Camera 1 tally PREVW");
      }
    }
  }
  if (debug) Serial.println();
}

void receive_C2_tally_PRG(OSCMessage &rcvmsg) {
  if (rcvmsg.isString(0)) {
    int length = rcvmsg.getDataLength(0);
    if (length < 16) {
      char str[length];
      rcvmsg.getString(0, str, length);
     
      if (length == 4) {
        setLedBlack(1);
        if (debug) Serial.println("Camera 2 tally OFF");
      }

      if (length == 5) {
        setLedRed(1);
        if (debug) Serial.println("Camera 2 tally PROG");
      }

      if (length == 6) {
        setLedBlack(1);
        if (debug) Serial.println("Camera 2 tally PREVW");
      }

    }
  }
  if (debug) Serial.println();
}

void receive_C3_tally_PRG(OSCMessage &rcvmsg) {
  if (rcvmsg.isString(0)) {
    int length = rcvmsg.getDataLength(0);
    if (length < 16) {
      char str[length];
      rcvmsg.getString(0, str, length);
     
      if (length == 4) {
        setLedBlack(2);
        if (debug) Serial.println("Camera 3 tally OFF");
      }

      if (length == 5) {
        setLedRed(2);
        if (debug) Serial.println("Camera 3 tally PROG");
      }

      if (length == 6) {
        setLedBlack(2);
        if (debug) Serial.println("Camera 3 tally PREVW");
      }

    }
  }
  if (debug) Serial.println();
}

void receive_C4_tally_PRG(OSCMessage &rcvmsg) {
  if (rcvmsg.isString(0)) {
    int length = rcvmsg.getDataLength(0);
    if (length < 16) {
      char str[length];
      rcvmsg.getString(0, str, length);
     
      if (length == 4) {
        setLedBlack(3);
        if (debug) Serial.println("Camera 4 tally OFF");
      }

      if (length == 5) {
        setLedRed(3);
        if (debug) Serial.println("Camera 4 tally PROG");
      }

      if (length == 6) {
        setLedBlack(3);
        if (debug) Serial.println("Camera 4 tally PREVW");
      }
    }
  }
  if (debug) Serial.println();
}

void receive_C5_tally_PRG(OSCMessage &rcvmsg) {
  if (rcvmsg.isString(0)) {
    int length = rcvmsg.getDataLength(0);
    if (length < 16) {
      char str[length];
      rcvmsg.getString(0, str, length);
     
      if (length == 4) {
        setLedBlack(4);
        if (debug) Serial.println("Camera 5 tally OFF");
      }

      if (length == 5) {
        setLedRed(4);
        if (debug) Serial.println("Camera 5 tally PROG");
      }

      if (length == 6) {
        setLedBlack(4);
        if (debug) Serial.println("Camera 5 tally PREVW");
      }
    }
  }
  if (debug) Serial.println();
}

void receive_C6_tally_PRG(OSCMessage &rcvmsg) {
  if (rcvmsg.isString(0)) {
    int length = rcvmsg.getDataLength(0);
    if (length < 16) {
      char str[length];
      rcvmsg.getString(0, str, length);
     
      if (length == 4) {
        setLedBlack(5);
        if (debug) Serial.println("Camera 6 tally OFF");
      }

      if (length == 5) {
        setLedRed(5);
        if (debug) Serial.println("Camera 6 tally PROG");
      }

      if (length == 6) {
        setLedBlack(5);
        if (debug) Serial.println("Camera 6 tally PREVW");
      }
    }
  }
  if (debug) Serial.println();
}

////////////////// Tally PRV /////////////////////////

void receive_C1_tally_PRV(OSCMessage &rcvmsg) {
  if (rcvmsg.isString(0)) {
    int length = rcvmsg.getDataLength(0);
    if (length < 16) {
      char str[length];
      rcvmsg.getString(0, str, length);
     
      if (length == 4) {
        setLedBlack(0);
        if (debug) Serial.println("Camera 1 tally OFF");
      }

      if (length == 5) {
        setLedGreen(0);
        if (debug) Serial.println("Camera 1 tally PROG");
      }

      if (length == 6) {
        setLedBlack(0);
        if (debug) Serial.println("Camera 1 tally PREVW");
      }
    }
  }
  if (debug) Serial.println();
}

void receive_C2_tally_PRV(OSCMessage &rcvmsg) {
  if (rcvmsg.isString(0)) {
    int length = rcvmsg.getDataLength(0);
    if (length < 16) {
      char str[length];
      rcvmsg.getString(0, str, length);
     
      if (length == 4) {
        setLedBlack(1);
        if (debug) Serial.println("Camera 2 tally OFF");
      }

      if (length == 5) {
        setLedGreen(1);
        if (debug) Serial.println("Camera 2 tally PROG");
      }

      if (length == 6) {
        setLedBlack(1);
        if (debug) Serial.println("Camera 2 tally PREVW");
      }

    }
  }
  if (debug) Serial.println();
}

void receive_C3_tally_PRV(OSCMessage &rcvmsg) {
  if (rcvmsg.isString(0)) {
    int length = rcvmsg.getDataLength(0);
    if (length < 16) {
      char str[length];
      rcvmsg.getString(0, str, length);
     
      if (length == 4) {
        setLedBlack(2);
        if (debug) Serial.println("Camera 3 tally OFF");
      }

      if (length == 5) {
        setLedGreen(2);
        if (debug) Serial.println("Camera 3 tally PROG");
      }

      if (length == 6) {
        setLedBlack(2);
        if (debug) Serial.println("Camera 3 tally PREVW");
      }

    }
  }
  if (debug) Serial.println();
}

void receive_C4_tally_PRV(OSCMessage &rcvmsg) {
  if (rcvmsg.isString(0)) {
    int length = rcvmsg.getDataLength(0);
    if (length < 16) {
      char str[length];
      rcvmsg.getString(0, str, length);
     
      if (length == 4) {
        setLedBlack(3);
        if (debug) Serial.println("Camera 4 tally OFF");
      }

      if (length == 5) {
        setLedGreen(3);
        if (debug) Serial.println("Camera 4 tally PROG");
      }

      if (length == 6) {
        setLedBlack(3);
        if (debug) Serial.println("Camera 4 tally PREVW");
      }
    }
  }
  if (debug) Serial.println();
}

void receive_C5_tally_PRV(OSCMessage &rcvmsg) {
  if (rcvmsg.isString(0)) {
    int length = rcvmsg.getDataLength(0);
    if (length < 16) {
      char str[length];
      rcvmsg.getString(0, str, length);
     
      if (length == 4) {
        setLedBlack(4);
        if (debug) Serial.println("Camera 5 tally OFF");
      }

      if (length == 5) {
        setLedGreen(4);
        if (debug) Serial.println("Camera 5 tally PROG");
      }

      if (length == 6) {
        setLedBlack(4);
        if (debug) Serial.println("Camera 5 tally PREVW");
      }
    }
  }
  if (debug) Serial.println();
}

void receive_C6_tally_PRV(OSCMessage &rcvmsg) {
  if (rcvmsg.isString(0)) {
    int length = rcvmsg.getDataLength(0);
    if (length < 16) {
      char str[length];
      rcvmsg.getString(0, str, length);
     
      if (length == 4) {
        setLedBlack(5);
        if (debug) Serial.println("Camera 6 tally OFF");
      }

      if (length == 5) {
        setLedGreen(5);
        if (debug) Serial.println("Camera 6 tally PROG");
      }

      if (length == 6) {
        setLedBlack(5);
        if (debug) Serial.println("Camera 6 tally PREVW");
      }
    }
  }
  if (debug) Serial.println();
}

///////////////////////////////////////////////////////////

void reset_system(OSCMessage &rcvmsg){
  if (rcvmsg.isString(0)) {
  if(debug)Serial.println("Restart !");
  ESP.restart();
  }
}

void wifi_reset(OSCMessage &rcvmsg){
  if (rcvmsg.isString(0)) {
  if(debug)Serial.println("Reset WiFi settings!");
  wm.resetSettings();
  delay(1000);
  ESP.restart();
  }
}

