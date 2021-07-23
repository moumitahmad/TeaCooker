#include "excecute.h"

const int dt = 15;

// teabag - servo motor
#include <Servo.h>
#define SERVO_PIN 7
Servo servoTeabag;
const int ANGLE_START = 10;
const int ANGLE_END = 180;


void transportTeabagToCup() {  
  for(int angle=ANGLE_START; angle<ANGLE_END; angle++) {
    servoTeabag.write(angle);
    delay(dt);
  }
}

void removeTeabagFromCup() {
  for(int angle = ANGLE_END; angle>ANGLE_START; angle--) {
    servoTeabag.write(angle);
    delay(dt);
  }
}

void excecuteProgramm(const Tea* tea) {
  servoTeabag.attach(SERVO_PIN);
  // bring teabag to cup
  transportTeabagToCup();

  // wait for brew time
  delay(60000*tea->m_brewingTime);

  // clean up
  removeTeabagFromCup();
}
