#include <RingBuf.h>
#include <Adafruit_MAX31855.h>
#include "atmega-adc/atmega-adc.h"

// pin assigments are random
#define TC_DO 20
#define TC_CLK 21
#define TC_CS_R1 22
#define TC_CS_R2 23
#define TC_CS_R3 24

#define PT_1 1
#define PT_2 2

Adafruit_MAX31855 TC_R1(TC_CLK, TC_CS_R1, TC_DO);
Adafruit_MAX31855 TC_R2(TC_CLK, TC_CS_R2, TC_DO);
Adafruit_MAX31855 TC_R3(TC_CLK, TC_CS_R3, TC_DO);

void setup() {
  
  delay(500);
  if (!TC_R1.begin()) {
    error("Thermocouple R1 error.");
  }
  if (!TC_R2.begin()) {
    error("Thermocouple R2 error.");
  }
  if (!TC_R3.begin()) {
    error("Thermocouple R3 error.");
  }
  delay(500);
  double c = TC_R1.readCelsius();
  if (isnan(c)) {
    error("TC_R1 Thermocouple fault( detected!");
    uint8_t e = TC_R1.readError();
    if (e & MAX31855_FAULT_OPEN) error("TC_R1 FAULT: Thermocouple is open - no connections.");
    if (e & MAX31855_FAULT_SHORT_GND) Serial.println("TC_R1 FAULT: Thermocouple is short-circuited to GND.");
    if (e & MAX31855_FAULT_SHORT_VCC) Serial.println("TC_R1 FAULT: Thermocouple is short-circuited to VCC.");
  }
  c = TC_R2.readCelsius();
  if (isnan(c)) {
    error("TC_R2 Thermocouple fault(s) detected!");
    uint8_t e = TC_R2.readError();
    if (e & MAX31855_FAULT_OPEN) error("TC_R2 FAULT: Thermocouple is open - no connections.");
    if (e & MAX31855_FAULT_SHORT_GND) error("TC_R2 FAULT: Thermocouple is short-circuited to GND.");
    if (e & MAX31855_FAULT_SHORT_VCC) error("TC_R2 FAULT: Thermocouple is short-circuited to VCC.");
  }
  c = TC_R3.readCelsius();
  if (isnan(c)) {
    error("TC_R3 Thermocouple fault(s) detected!");
    uint8_t e = TC_R3.readError();
    if (e & MAX31855_FAULT_OPEN) error("TC_R3 FAULT: Thermocouple is open - no connections.");
    if (e & MAX31855_FAULT_SHORT_GND) error("TC_R3 FAULT: Thermocouple is short-circuited to GND.");
    if (e & MAX31855_FAULT_SHORT_VCC) error("TC_R3 FAULT: Thermocouple is short-circuited to VCC.");
  }

  // set  up interrupts for PT ADC
  adc_start(ADC_PRESCALER_128, ADC_VREF_AVCC, 2, PT_interrupt);
  sei();
}

#define PT_1_CONS 200
#define PT_2_CONS 200

RingBuf<float, 100> PT_1_buf;
RIngBuf<float, 100> PT_2_buf;

void PT_interrupt(uint8_t pin, uint16_t value) {
  switch(pin) {
    case PT_1: {
        float val = value/1024 * PT_1_CONS;
        PT_1_buf.push(val);
    };
    case PT_2: {
        float val = value/1024 * PT_2_CONS;
        PT_2_buf.push(val);
    };
    default: {
      // do nothing
    };
  }
}



void error(String error) {

}

void loop() {
  // put your main code here, to run repeatedly:

}
