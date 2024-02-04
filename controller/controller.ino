#include "RS485_protocol.h"
#include "protocol.h"
#include <HardwareSerial.h>

const byte ENABLE_PIN = 22;

// callback routines
  
void fWrite (const byte what){
  Serial1.write(what);  
  /*while (!(UCSR1A & (1 << UDRE1))){
    UCSR1A |= 1 << TXC1;
  }
  while (!(UCSR1A & (1 << TXC1)));*/
}
  
int fAvailable(){
  return Serial1.available();  
}

int fRead(){
  
  int data = Serial1.read();  
  return data;
}

void setup(){
  Serial1.begin (9600);
  Serial.begin(9600);
  pinMode (ENABLE_PIN, OUTPUT);  // driver output enable
  digitalWrite(ENABLE_PIN, LOW);
}  // end of setup
  
unsigned long counter = 0;

int attempts = 0;

void printPacket(Response * packet) {
    Serial.print("Header: ");
    Serial.println(packet->header & 0xFF, HEX);
    Serial.print("Type: ");
    Serial.println(packet->type);
    Serial.print("Counter: ");
    Serial.println(packet->counter);    
}

void loop(){
  Response packet;

  // receive response  
  byte received = recvMsg (fAvailable, fRead, (char*) &packet, sizeof(Response));

  if (received == sizeof(Response)){
    printPacket(&packet);
  }

}  // end of loop

