#include "RS485_protocol.h"
#include "protocol.h"
#include <stdint.h>
#include <Adafruit_MAX31855.h>

#define TC_DO 51
#define TC_CLK 52
#define TC_CS_R1 40
#define TC_CS_R2 41

Adafruit_MAX31855 TC_R1(TC_CLK, TC_CS_R1, TC_DO);
Adafruit_MAX31855 TC_R2(TC_CLK, TC_CS_R2, TC_DO);

void error(String error) {
  Serial.println(error);
}



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

  delay(500);
  if (!TC_R1.begin()) {
    error("Thermocouple R1 error.");
  }
  Serial.println("Thermo 1 complete");
  if (!TC_R2.begin()) {
    error("Thermocouple R2 error.");
  }
  Serial.println("Thermo 2 complete");


  delay(500);
  double c = TC_R1.readCelsius();
  if (isnan(c)) {
    error("TC_R1 Thermocouple fault( detected!");
    uint8_t e = TC_R1.readError();
    if (e & MAX31855_FAULT_OPEN) error("TC_R1 FAULT: Thermocouple is open - no connections.");
    if (e & MAX31855_FAULT_SHORT_GND) Serial.println("TC_R1 FAULT: Thermocouple is short-circuited to GND.");
    if (e & MAX31855_FAULT_SHORT_VCC) Serial.println("TC_R1 FAULT: Thermocouple is short-circuited to VCC.");
  }
  Serial.println("Thermo 1 read complete");
  Serial.println(c);


  c = TC_R2.readCelsius();
  if (isnan(c)) {
    error("TC_R2 Thermocouple fault(s) detected!");
    uint8_t e = TC_R2.readError();
    if (e & MAX31855_FAULT_OPEN) error("TC_R2 FAULT: Thermocouple is open - no connections.");
    if (e & MAX31855_FAULT_SHORT_GND) error("TC_R2 FAULT: Thermocouple is short-circuited to GND.");
    if (e & MAX31855_FAULT_SHORT_VCC) error("TC_R2 FAULT: Thermocouple is short-circuited to VCC.");
  }

  Serial.println("Thermo 2 read complete");
  Serial.println(c);

}

unsigned long counter = 0;

int attempts = 0;

byte buf [18];

void loop(){
  byte received = recvMsg (fAvailable, fRead, buf, sizeof (buf));

  if (received){
    Response * packet_ptr = (Response *) buf;
    Serial.print("header: ");
    Serial.println(packet_ptr->header);
    Serial.print("type: ");
    Serial.println(packet_ptr->type);
    Serial.print("counter: ");
    Serial.println(packet_ptr->counter);
  }

  Response packet;

  packet.type = 1;
  packet.counter = counter;
  packet.tc1 = TC_R1.readCelsius();
  packet.tc2 = TC_R2.readCelsius();
  packet.pt1 = analogRead(A0);
  packet.pt2 = analogRead(A1);

  counter++;

  byte * packet_addr = (byte *)(&packet);
  
  delay (1);  // give the master a moment to prepare to receive
  sendMsg(fWrite, packet_addr, sizeof(packet));    
}  // end of loop