#include "heating.h"

const int relayPin = 7;
const double MIN_TEMP = 60;
const double MAX_TEMP = 70;
bool heaterIsOn = false;

void setupHeating() {
  pinMode(relayPin, OUTPUT);
}

void controlHeating(double currentTempF) {
  if (heaterIsOn) {
    if (currentTempF >= MAX_TEMP) {
      digitalWrite(relayPin, LOW);
      heaterIsOn = false;
    }
  }
  else {
    if (currentTempF <= MIN_TEMP) {
      digitalWrite(relayPin, HIGH);
      heaterIsOn = true;
    }
  }
}
