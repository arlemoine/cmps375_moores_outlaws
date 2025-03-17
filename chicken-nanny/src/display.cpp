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
  lcd.print("F");
}
  
void updateDisplay(double currentTempF) {
  Serial.print("Temperature: ");
  Serial.print(currentTempF, 1);
  Serial.println(" °F");

  lcd.setCursor(6, 0);
  // TODO: add code to print time here (on first line of the lcd)

  int roundedTemp = round(currentTempF);
  lcd.setCursor(6, 1);
  lcd.print(roundedTemp);
  lcd.setCursor(10, 1);
  if (isHeaterOn()) {
    lcd.print("HTR ON");
  }
  else {
    lcd.print("HTR OFF");
  }
}