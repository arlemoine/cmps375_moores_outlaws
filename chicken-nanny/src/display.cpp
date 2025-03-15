#include "display.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setupDisplay() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Temperature: ");
  lcd.setCursor(5, 1);
  lcd.print(char(223)); //print degree symbol (°)
  lcd.print("F");
}
  
void updateDisplay(double currentTempF) {
  Serial.print("Temperature: ");
  Serial.print(currentTempF, 1);
  Serial.println(" °F");
  
  lcd.setCursor(0, 1);
  lcd.print(currentTempF, 1);
}