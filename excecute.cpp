#include "excecute.h"

const int dt = 15;

// servo-motor
#include <Servo.h>
#define SERVO_PIN 8
Servo servo;
const int ANGLE_START = 10;
const int ANGLE_END = 180;


void transportTeabagToCup() {
  servo.attach(SERVO_PIN);
  servo.write(ANGLE_START);
  
  for(int angle=ANGLE_START; angle<ANGLE_END; angle++) {
    servo.write(angle);
    delay(dt);
  }
}

void removeTeabagFromCup() {
  for(int angle = ANGLE_END; angle>ANGLE_START; angle--) {
    servo.write(angle);
    delay(dt);
  }
}

void excecuteProgramm(const Tea* tea) {
  // bring teabag to cup
  transportTeabagToCup();

  // wait for brew time
  delay(60000*tea->m_brewingTime);

  // clean up
  removeTeabagFromCup();
}
