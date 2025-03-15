#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

// Function Prototypes
void setupDisplay();
void updateDisplay(double currentTempF);

#endif