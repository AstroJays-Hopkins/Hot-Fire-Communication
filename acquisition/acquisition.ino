#include "RS485_protocol.h"
#include "protocol.h"
#include <stdint.h>


void error(String error) {
  Serial.println(error);
}

const byte ENABLE_PIN = 22;

void fWrite(const byte what){ 
  Serial1.write(what);    
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
  Serial1.begin(9600);
  Serial.begin(9600);
  pinMode(ENABLE_PIN, OUTPUT);  // driver output enable
  digitalWrite(ENABLE_PIN, HIGH);
}

unsigned long counter = 0;

int attempts = 0;


void loop(){
  Response packet;

  packet.type = 1;
  packet.counter = counter;
  packet.tc1 = 0;
  packet.tc2 = 0;
  packet.pt1 = 0;
  packet.pt2 = 0;

  counter++;

  // toggle the direction for duration of the entire packet
  digitalWrite(ENABLE_PIN, HIGH);
  sendMsg(fWrite, (char*) &packet, sizeof(Response));

  // wait until TX buffer has emptied before sending another packet
  while (!(UCSR1A & (1 << UDRE1))){
    UCSR1A |= 1 << TXC1;
  }
  while (!(UCSR1A & (1 << TXC1)));
  digitalWrite(ENABLE_PIN, LOW);

}  // end of loop