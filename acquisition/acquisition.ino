#include "RS485_protocol.h"

const byte ENABLE_PIN = 2;

void fWrite(const byte what){
  Serial1.write(what);  
}
  
int fAvailable(){
  return Serial1.available();  
}

int fRead(){
  return Serial1.read();  
}
  
void setup(){
  Serial1.begin(115200);
  Serial.begin(9600);
  pinMode(ENABLE_PIN, OUTPUT);  // driver output enable
  digitalWrite(ENABLE_PIN, LOW);
}

void loop(){
  byte buf [7];
  
  byte received = recvMsg (fAvailable, fRead, buf, sizeof (buf));

  buf[6] = 0;
  Serial.println((char *)buf);
  
}  // end of loop