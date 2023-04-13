#include "RS485_protocol.h"
#include "protocol.h"
#include <stdint.h>


const byte ENABLE_PIN = 2;

void fWrite(const byte what){
  digitalWrite(ENABLE_PIN, HIGH);
  Serial1.write(what);  

  while (!(UCSR1A & (1 << UDRE1))){
    UCSR1A |= 1 << TXC1;
  }
  while (!(UCSR1A & (1 << TXC1)));

  digitalWrite (ENABLE_PIN, LOW);

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
}

unsigned long counter = 0;

enum CommStateMachine {SENDING, WAITING};

CommStateMachine commstate = CommStateMachine::WAITING;

byte buf [10];

int attempts = 0;

void loop(){
  Serial.print("State:");
  Serial.println(commstate);
  switch(commstate){
    case CommStateMachine::WAITING:
      byte received = recvMsg(fAvailable, fRead, buf, sizeof (buf));
      if(received){
        Response * packet_ptr = (Response *) buf;
        Serial.print("header: ");
        Serial.println(packet_ptr->header);
        Serial.print("type: ");
        Serial.println(packet_ptr->type);
        Serial.print("counter: ");
        Serial.println(packet_ptr->counter);

        commstate = CommStateMachine::SENDING;
      } 
      else{
        delay(1);
      }
      break;
    
    case CommStateMachine::SENDING:
      Response packet;

      packet.type = 1;
      packet.counter = counter;

      counter++;

      byte * packet_addr = (byte *)(&packet);

      // send to slave  
      sendMsg(fWrite, packet_addr, sizeof(packet));

      commstate = CommStateMachine::WAITING;
      break;
  }
}  // end of loop