#include "RS485_protocol.h"

const byte ENABLE_PIN = 4;

void fWrite(const byte what){
  Serial.write(what);  
}
  
int fAvailable(){
  return Serial.available();  
}

int fRead(){
  return Serial.read();  
}
  
void setup(){
  Serial.begin (28800);
  pinMode (ENABLE_PIN, OUTPUT);  // driver output enable
}

void loop(){
  byte buf [10];
  
  byte received = recvMsg (fAvailable, fRead, buf, sizeof (buf));
  
  if (received){
    if (buf [0] != 1)
      return;  // not my device
      
    if (buf [1] != 2)
      return;  // unknown command
    
    byte msg [] = {
       0,  // device 0 (master)
       3,  // turn light on command received
    };
    
    delay (1);  // give the master a moment to prepare to receive
    digitalWrite (ENABLE_PIN, HIGH);  // enable sending
    sendMsg (fWrite, msg, sizeof msg);

    while (!(UCSR0A & (1 << UDRE0)))  // Wait for empty transmit buffer
    UCSR0A |= 1 << TXC0;  // mark transmission not complete
    while (!(UCSR0A & (1 << TXC0)));   // Wait for the transmission to complete

    digitalWrite (ENABLE_PIN, LOW);  // disable sending
  }  // end if something received
}  // end of loop