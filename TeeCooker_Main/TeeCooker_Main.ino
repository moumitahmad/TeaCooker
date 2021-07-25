#include "dataService.h"
#include "setup.h"
#include "excecute.h"
#include "userFeedback.h"

// Phase defines the current marchine state
enum Phase {
  SETUP,
  EXCECUTE,
  FEEDBACK
};
Phase phase = SETUP;
Tea* tea;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  switch (phase) {
    case SETUP:
      Serial.println("Start phase SETUP.");
      tea = startProgram();
      phase = EXCECUTE;
      break;
    case EXCECUTE:
      Serial.println("Start phase EXCECUTE.");
      excecuteProgramm(tea);
      phase = FEEDBACK;
      break;
    case FEEDBACK:
      Serial.println("Start phase FEEDBACK.");
      finishProgram(tea);
      break;
    default:
      Serial.println("The choosen phase is not defined.");
  }
  delay(500);
}
