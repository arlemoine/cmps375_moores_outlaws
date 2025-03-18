#include "display.h"
#include "heating.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setupDisplay() {
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Time: ");

  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.setCursor(8, 1);
  lcd.print(char(223)); //print degree symbol (°)
}
  
void updateDisplay(double currentTempF) {
  Serial.print("Temperature: ");
  Serial.print(currentTempF, 1);
  Serial.println(" °");

  lcd.setCursor(6, 0);
  dt = clock.now();
  lcd.print(dt.hour());
  lcd.print(":");
  lcd.print(dt.minute());

  int roundedTemp = round(currentTempF);
  lcd.setCursor(6, 1);
  lcd.print(roundedTemp);
  lcd.setCursor(9, 1);
  if (isHeaterOn()) {
    lcd.print("HTR ON ");
  }
  else {
    lcd.print("HTR OFF");
  }
}