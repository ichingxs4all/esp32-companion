OSCMessage msgA("/location/1/0/1/press");
OSCMessage msgB("/location/1/0/1/rotate-left");
OSCMessage msgC("/location/1/0/1/rotate-right");


void sendOSCA(){
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
