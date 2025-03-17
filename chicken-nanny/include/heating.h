#ifndef HEATING_H
#define HEATING_H

#include <Arduino.h>

// Function Prototypes
void setupHeating();
void controlHeating(double currentTempF);
bool isHeaterOn();

#endif