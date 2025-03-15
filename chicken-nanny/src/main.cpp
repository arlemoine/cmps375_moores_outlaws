#include <Arduino.h>
#include <heating.h>
#include <feeder.h>
#include <display.h>
#include <max6675.h>

const int sck = 12;
const int cs = 10;
const int so = 4;
MAX6675 tempSensor(sck, cs, so);

void setup() {
  Serial.begin(9600);

  setupDisplay();
  setupFeeder();
  setupHeating();
  
  delay(1000);
}

void loop() {
  double currentTempF = tempSensor.readFahrenheit();

  controlFeeder();
  controlHeating(currentTempF);
  updateDisplay(currentTempF);

  delay(10000);
}