
#include "RS485_protocol.h"

const byte ENABLE_PIN = 4;

void fWrite (const byte what){
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
  pinMode (LED_PIN, OUTPUT);  // built-in LED
}  // end of setup
  
byte old_level = 0;

void loop(){
  // send to slave  
  digitalWrite (ENABLE_PIN, HIGH);  // enable sending
  sendMsg (fWrite, msg, sizeof msg);

  while (!(UCSR0A & (1 << UDRE0)))  // Wait for empty transmit buffer
  UCSR0A |= 1 << TXC0;  // mark transmission not complete
  while (!(UCSR0A & (1 << TXC0)));   // Wait for the transmission to complete

  digitalWrite (ENABLE_PIN, LOW);  // disable sending

  // receive response  
  byte buf [10];
  byte received = recvMsg (fAvailable, fRead, buf, sizeof buf);
}  // end of loop