#include <RingBuf.h>

void setup() {
  Serial1.begin(9600);
  Serial.begin(9600);
}

//char buffer[6];

RingBuf<byte, 6> buf;

  char buffer[6];

void loop() {
  // if we get a read_request
  receivechar();
  for(int i = 0; i < 6; i++) {
    //Serial.print(buffer[i]);
  }
  //Serial.println();
  //if(buffer == "hello\n\0") {
    //Serial.println("received handshake");
  //}

  Serial.println("Trying to read 6 bytes");
  for(int i = 0; i < 6; i++) {
    uint8_t c;
    if(buf.pop(c)) {
      buffer[i] = (char)c;
      Serial.print("Read ");
      Serial.print(c);
      Serial.print(" which is ");
      Serial.print((char) c);
      Serial.println(" into buffer");
    }
  }
  Serial.println("");
  if(buffer == "hello\n") {
    //Serial.println("received handshake");
  }
  for(int i = 0; i < 6; i++) {
    //Serial.print((uint8_t) buffer[i]);
    //Serial.print(" ");
  }
  //Serial.println();
  //Serial.print(buffer);
}



void receivechar() {
  digitalWrite(3, LOW); // transmit on 
  digitalWrite(2, HIGH); // receive off
  int i = 0;

  
  //Serial1.readBytes(buffer, 6);
  buf.push(Serial1.read());

  //Serial1.flush();
}