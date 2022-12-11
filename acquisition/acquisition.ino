#include <RingBuf.h>

void setup() {
  Serial1.begin(9600);
  Serial.begin(9600);
  Serial.print("Acquisiton starting.");
}


//RingBuf<uint8_t, 6> buf;

#define SIG 0x53
uint8_t buffer[7];
int pos;

enum state_t {IDLE, RECEIVING, READY};


state_t state = IDLE;

unsigned long time_start = 0;
int period = 100;

void loop() {
  // if we get a read_request
  //receivechar();
  switch(state) {
    case RECEIVING: {
      while(Serial1.available()) {
          buffer[pos] = Serial1.read();
          if(buffer[0] != SIG) {
            pos = 0;            
            continue; // keep going till we get a packet starting with the signature
          }
          pos++;
          if(pos > 7) {
            pos = 0;
            if(buffer[0] == SIG) {
              time_start = millis();
              state = READY;
            }
              
            
            break;

          }
        }
    };
    case READY: {
      for(int i = 1; i < 6; i++) {
        Serial.write(buffer[i]);
        delay(1);
      }
      Serial.println();
      memset(buffer, 0, 7); // clear the buffer for now
      state = IDLE;
    };
    case IDLE: {
      if(millis() > time_start + period) {
        //Serial1.flush();
        state = RECEIVING;
      }
    };
  }
}



void receivechar() {
  digitalWrite(3, LOW); // transmit on 
  digitalWrite(2, HIGH); // receive off
  int i = 0;

  
  //Serial1.readBytes(buffer, 6);
  //buf.push(Serial1.read());

  //Serial1.flush();
}