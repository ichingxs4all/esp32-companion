void sendOSCA(){
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Iris");

    //Udp.beginPacket(outIp, outPort);
    Udp.beginPacket(outIp, outPort);
    msgA.send(Udp);
    Udp.endPacket();
    msgA.empty();  
}

void sendOSCB(){
    Udp.beginPacket(outIp, outPort);
    msgB.send(Udp);
    Udp.endPacket();
    msgB.empty();  
}

void sendOSCC(){
    Udp.beginPacket(outIp, outPort);
    msgC.send(Udp);
    Udp.endPacket();
    msgC.empty();  
}
