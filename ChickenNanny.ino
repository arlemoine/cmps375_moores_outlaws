#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <max6675.h>
#include <Servo.h>
#include <DS3231.h>
#include <RTClib.h>
#include <Keypad.h>

// define relay pin
int relayPin = 7;
const double MIN_TEMP = 65;
const double MAX_TEMP = 70;
bool heaterIsOn = false;

// define servo variables
Servo myservo;
int pos = 0;

// define thermocouple pins
const int sck = 12;
const int cs = 10;
const int so = 4;

MAX6675 thermocouple(sck, cs, so);
LiquidCrystal_I2C lcd(0x27, 16, 2);

//define time for clock
RTC_DS3231 clock;
DateTime dt;
int lastMinute = -1;

const byte COL = 4;
const byte ROW = 4;
char keys[ROW][COL] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROW] = {13, 11, 8, 6};
byte colPins[COL] = {5, 3, 2, 1};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROW, COL);

int timeIndex = 0;
char inputTime[5];
int feedTimes[20][2];
int feedTimesCounter = 0;
int numFeedTimes = sizeof(feedTimes) / sizeof(feedTimes[0]);

bool inputMethod = false;

void setup() {
  pinMode(relayPin, OUTPUT);
  myservo.write(0);

  setupDisplay();
  setupFeeder();
  setupHeating();

  clock.begin();

  if(clock.lostPower()){
    clock.adjust(DateTime(__DATE__ , __TIME__));
  }
    delay(1000);
}

void loop() {
  double currentTempF = thermocouple.readFahrenheit();
  dt = clock.now();
  controlFeeder();
  controlHeating(currentTempF);
  if(inputMethod == false){
    updateDisplay(currentTempF);
  }

  char key = keypad.getKey();
  if(key != NO_KEY){
    if((isdigit(key)) && (timeIndex < 4)){
      inputMethod = true;
        inputTime[timeIndex] = key;
        lcd.setCursor(0,0);
        lcd.print("Input Feed Time:");
        lcd.setCursor(timeIndex++, 1);
        lcd.print(key);
        lcd.print("                ");
        if(timeIndex == 20){
          timeIndex = 0;
        }
    }
    else if(key == '#'){
      if(timeIndex == 4){
        inputTime[4] = '\0';
        int hour = (inputTime[0] - '0') * 10 + (inputTime[1] - '0');
        int minute = (inputTime[2] - '0') * 10 + (inputTime[3] - '0');
        if ((hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59)) {
          feedTimes[feedTimesCounter][0] = hour;
          feedTimes[feedTimesCounter][1] = minute;
          feedTimesCounter++;
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("New Feeding Time");
          lcd.setCursor(0,1);
          lcd.print(hour);
          lcd.print(":");
          lcd.print(minute);
          delay(2000);
          setupDisplay();
          inputMethod = false;
        } else {
          lcd.setCursor(0, 1);
          lcd.print("Invalid Time    ");
          delay(2000);
          setupDisplay();
          inputMethod = false;
      }
    }
    timeIndex = 0;
  }
  else if(key == '*'){
    inputMethod = false;
    timeIndex = 0;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Input Cancelled");
    delay(2000);
    lcd.clear();
    setupDisplay();
  }
}
delay(150);
}

void setupFeeder(){
  myservo.attach(9);
}

void controlFeeder() {
  int currentHour = dt.hour();
  int currentMinute = dt.minute();
  for(int i = 0; i < numFeedTimes; i++){
    if((currentHour == feedTimes[i][0]) && (currentMinute == feedTimes[i][1])){
      if(currentMinute != lastMinute){
        rotateServo();
        lastMinute = currentMinute;
      }
    }
  }
}

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

bool isHeaterOn() {
  return heaterIsOn;
}

void rotateServo() {
  for (pos = 0; pos <= 160; pos += 1) {
    myservo.write(pos);
    delay(5);
  }
  delay(3000);
  for (pos = 160; pos >= 0; pos -= 1) {
    myservo.write(pos);
    delay(5);
  }
}

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