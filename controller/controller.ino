
#include "RS485_protocol.h"

const byte ENABLE_PIN = 2;

// callback routines
  
void fWrite (const byte what){
  Serial1.write(what);  
}
  
int fAvailable(){
  return Serial1.available();  
}

int fRead(){
  return Serial1.read();  
}

void setup(){
  Serial1.begin (115200);
  pinMode (ENABLE_PIN, OUTPUT);  // driver output enable
}  // end of setup
  
void loop(){
  const byte msg [] = "Shello";

  // send to slave  
  digitalWrite(ENABLE_PIN, HIGH);  // enable sending
  sendMsg (fWrite, msg, sizeof(msg));
  delayMicroseconds (1220);
  digitalWrite (ENABLE_PIN, LOW);  // disable sending
  delay(10);
}  // end of loop