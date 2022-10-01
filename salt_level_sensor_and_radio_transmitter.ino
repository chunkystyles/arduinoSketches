#include <RH_ASK.h>
#include <SPI.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define USONIC_DIV 58.0
#define MEASURE_SAMPLE_DELAY 5
#define MEASURE_SAMPLES 25
#define MEASURE_DELAY 250

#define echoPin 15
#define trigPin 14
#define wait 60000
#define messageSize 10

long distance;

// (speed, rx pin, tx pin, ptt pin, ptt inverted)
RH_ASK driver(2000, 9, 10, 8, false);

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(19200);
  if (!driver.init()){
    Serial.println("init failed");
  }
}

void loop() {
  delay(wait);
  distance = measure();
  char message[messageSize];
  sprintf(message, "%d", distance);
  driver.send(message, messageSize);
  driver.waitPacketSent();
}

long measure() {
  long measureSum = 0;
  for (int i = 0; i < MEASURE_SAMPLES; i++)
  {
    delay(MEASURE_SAMPLE_DELAY);
    measureSum += singleMeasurement();
  }
  return measureSum / MEASURE_SAMPLES;
}

long singleMeasurement() {
  long duration = 0;
  // Measure: Put up Trigger...
  digitalWrite(trigPin, HIGH);
  // ... wait for 11 µs ...
  delayMicroseconds(11);
  // ... put the trigger down ...
  digitalWrite(trigPin, LOW);
  // ... and wait for the echo ...
  duration = pulseIn(echoPin, HIGH);
  return (long) (((float) duration / USONIC_DIV) * 10.0);
}
