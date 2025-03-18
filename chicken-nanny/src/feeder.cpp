#include "feeder.h"

RTC_DS3231 clock;
DateTime dt;
int lastMinute = -1;

//Times where the feeder activates. 7:28, 7:29, 7:31, 7:34, 7:36
int feedTimes[][2] = { {19, 28}, {19, 29}, {19, 31}, {19, 34}, {19, 36}}; 
int numFeedTimes = sizeof(feedTimes) / sizeof(feedTimes[0]);

Servo myservo;
int pos = 0;

void rotateServo() {
  for (pos = 0; pos <= 180; pos += 1) {
    myservo.write(pos);
    delay(15);
  }
  for (pos = 180; pos >= 0; pos -= 1) {
    myservo.write(pos);
    delay(15);
  }
}

void setupFeeder() {
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