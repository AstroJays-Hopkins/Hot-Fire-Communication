#include "RS485_protocol.h"
#include "protocol.h"

const byte ENABLE_PIN = 2;

// callback routines
  
void fWrite (const byte what){
  digitalWrite(ENABLE_PIN, HIGH);
  Serial1.write(what);  

  while (!(UCSR1A & (1 << UDRE1))){
    UCSR1A |= 1 << TXC1;
  }
  while (!(UCSR1A & (1 << TXC1)));

  digitalWrite(ENABLE_PIN, LOW);
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
  Serial1.begin (115200);
  Serial.begin(9600);
  pinMode (ENABLE_PIN, OUTPUT);  // driver output enable
}  // end of setup
  
unsigned long counter = 0;

enum CommStateMachine {SENDING, WAITING};

CommStateMachine commstate = CommStateMachine::SENDING;

int attempts = 0;

byte buf [18];

void loop(){
  Response packet;

  packet.type = 2;
  packet.counter = counter;
  packet.tc1 = 0;
  packet.tc2 = 0;
  packet.pt1 = 0;
  packet.pt2 = 0;

  counter++;

  byte * packet_addr = (byte *)(&packet);

  // send to slave  
  delay(1);
  sendMsg(fWrite, packet_addr, sizeof(packet));

  // receive response  
  byte received = recvMsg (fAvailable, fRead, buf, sizeof(buf));

  if (received){
    Response * packet_ptr = (Response *) buf;
    Serial.write((char * ) packet_ptr, 22);
    
  }

  
  
}  // end of loop




/*
Acquisition
Board - Tranciever
GND - GND
2 - RT
TX1 - RX
RX1 - TX
5V - 5V
*/

/*
Controller
Board - Tranciever
GND - GND
2 - RT
TX1 - RX
RX1 - TX
5V - 5V


*/