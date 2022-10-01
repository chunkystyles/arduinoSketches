#include <RH_ASK.h>
#include <SPI.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define state_on "on"
#define state_off "off"
#define messageSize 10
#define loopDelay 50
#define activationDelay 5000
#define threshold 300

bool wasOn = false;
// (speed, rx pin, tx pin, ptt pin, ptt inverted)
RH_ASK driver(2000, 10, 9, 8, false);
unsigned long last = 0;
bool isDelay = false;
bool isActivationDelay = false;
uint8_t message[messageSize];
uint8_t messageLength = messageSize;

void setup() {
  Serial.begin(9600);
  if (!driver.init()){
    Serial.println("init failed");
  }
  last = millis();
}

void loop() {
  checkBell();
  checkRadio();
}

void checkBell(){
  unsigned long now = millis();
  unsigned long difference = now - last;
  if ((isDelay && difference < loopDelay) || (isActivationDelay && difference < activationDelay)){
    return;
  } else {
    last = now;
    isDelay = false;
    isActivationDelay = false;
    int value = analogRead(A1);
    bool isOn = value > threshold;
    if (isOn){
      if (!wasOn){
        Serial.println(state_on);
        wasOn = true;
      }
      isActivationDelay = true;
    } else {
      if (wasOn){      
        Serial.println(state_off);
        wasOn = false;
      }
      isDelay = true;
    }
  }
}

void checkRadio(){
  if (driver.recv(message, &messageLength)){
    Serial.println((char*)message);         
  }
}
