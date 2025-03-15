#include "feeder.h"

Servo myservo;
int pos = 0;
long previousFeedTime = 0;
const long feedInterval = 6L * 60L * 60L * 1000L; // 6 hours in millis (The L is used to tell the compiler to use a long data type)

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
  long currentMillis = millis();
  if (currentMillis - previousFeedTime >= feedInterval) {
      rotateServo();
      previousFeedTime = currentMillis;
  }
}