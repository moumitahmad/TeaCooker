#include "userFeedback.h"

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Ticker.h>

#define YES_BUTTON_PIN 3
#define NO_BUTTON_PIN 2
#define YES_BUTTON_NOTE  1568
#define NO_BUTTON_NOTE  1319

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

// general
void turnOff() {
  lcd.clear();
  lcd.noDisplay();
  lcd.noBacklight();
}

void turnOn() {
  lcd.display();
  lcd.backlight();
}

void resetDisplay() {
  lcd.clear();
}

void displayMessage(char* line1, char* line2, int dt) {
  resetDisplay();
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.setCursor(0,0);
  lcd.print(line1);
  lcd.setCursor(0,1);
  lcd.print(line2);
  delay(dt);
}

void displayMessage(char* line1, double amount, int dt) {
  resetDisplay();
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.setCursor(0,0);
  lcd.print(line1);
  lcd.setCursor(0,1);
  lcd.print(amount);
  delay(dt);
}


// ASK USER
bool done = false;
bool answer;


void checkButton() {
  if(digitalRead(YES_BUTTON_PIN)==HIGH) { // button is clicked
    Serial.println("YES");
    answer = true;
    done = true;
  } else if(digitalRead(NO_BUTTON_PIN)==HIGH) { // button is clicked
    Serial.println("NO");
    answer = false;
    done = true;
  } else {
    Serial.println("NOTHING");
  }
}

void resetFeedback() {
  answer = false;
  done = false;
}

bool askUser(char* line1, char* line2) {
  resetFeedback();
  // setup
  pinMode(YES_BUTTON_PIN, INPUT_PULLUP);
  pinMode(NO_BUTTON_PIN, INPUT_PULLUP);  
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);

  while(!done) {
    checkButton();
    delay(300);
  }
  return answer;
}

void finishProgram(Tea* tea) {
  // setup
  //Serial.println(tea->m_name);
  lcd.begin(16, 2);
  // Print a message to the LCD.
  displayMessage("The tea is done!", "Enjoy :)", 300);
  
  int i=0;
  while(i<8) {
    delay(500);
    i++;
  }
  turnOff();
}
