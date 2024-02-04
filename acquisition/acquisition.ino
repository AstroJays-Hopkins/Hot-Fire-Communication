#include "RS485_protocol.h"
#include "protocol.h"
#include <stdint.h>
#include <Adafruit_MAX31855.h>

void error(String error) {
  Serial.println(error);
}

const byte ENABLE_PIN = 22;

void fWrite(const byte what){
  Serial1.write(what);  

  while (!(UCSR1A & (1 << UDRE1))){
    UCSR1A |= 1 << TXC1;
  }
  while (!(UCSR1A & (1 << TXC1)));
}
  
int fAvailable(){
  return Serial1.available();  
}

int fRead(){
  digitalWrite(ENABLE_PIN, LOW);
  int data = Serial1.read();  
  return data;
}
  
void setup(){
  Serial1.begin(115200);
  Serial.begin(9600);
  pinMode(ENABLE_PIN, OUTPUT);  // driver output enable
  digitalWrite(ENABLE_PIN, HIGH);
}

unsigned long counter = 0;

int attempts = 0;

byte buf [18];

void loop(){
  Response packet;

  packet.type = 1;
  packet.counter = counter;
  packet.tc1 = 0;
  packet.tc2 = 0;
  packet.pt1 = 0;
  packet.pt2 = 0;

  counter++;

  byte * packet_addr = (byte *)(&packet);
  
  sendMsg(fWrite, packet_addr, sizeof(packet));    
}  // end of loop