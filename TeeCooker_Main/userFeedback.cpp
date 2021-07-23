#include "userFeedback.h"

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Ticker.h>

#define YES_BUTTON_PIN 3
#define NO_BUTTON_PIN 2
#define SOUND_PIN 1

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

const int DISPLAY_WIDTH = 16;
int m_text_length = 0;

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

// Displayer
void scroll() {
  Serial.println("scroll");
  int scrollWidth = m_text_length + 6 - DISPLAY_WIDTH;
  // to move it offscreen left:
  for (int positionCounter = 0; positionCounter < scrollWidth; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayLeft();
    // wait a bit:
    delay(300);
  }

  // to move it offscreen right:
  for (int positionCounter = 0; positionCounter < scrollWidth; positionCounter++) {
    // scroll one position right:
    lcd.scrollDisplayRight();
    // wait a bit:
    delay(400);
  }
}

void displayMessage(char* message, int text_length) {
  turnOn();
  Serial.println(message);
  m_text_length = text_length;
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.setCursor(3,0);
  lcd.print(message);
  scroll();
  turnOff();
}


// ASK USER
volatile bool done = false;
volatile bool answer;


void noButtonPressed() {
  answer = false;
  done = true;
}

void yesButtonPressed() {
  answer = true;
  done = true;
}

void resetFeedback() {
  answer = false;
  done = false;
  detachInterrupt(0);
  detachInterrupt(1);
}

bool askUser(char* message, int text_length) {
  resetFeedback();
  turnOn();
  
  // setup
  pinMode(YES_BUTTON_PIN, INPUT_PULLUP);
  pinMode(NO_BUTTON_PIN, INPUT_PULLUP);
  
  attachInterrupt(0, noButtonPressed, RISING);
  attachInterrupt(1, yesButtonPressed, RISING);
  
  m_text_length = text_length;
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.setCursor(3,0);
  lcd.print(message);
  while(!done) {
    scroll();
  }
  turnOff();
  return answer;
}

void playSound() {
  // inspired by https://forum.arduino.cc/t/piezo-buzzer-win-and-fail-sound/133792
  int melody[] = {
  262, 196,196, 220, 196,0, 247, 262};
  // note durations: 4 = quarter note, 8 = eighth note, etc.:
  int noteDurations[] = {
  4, 8, 8, 4,4,4,4,4 };

    for (int thisNote = 0; thisNote < 8; thisNote++) {
      int noteDuration = 1000/noteDurations[thisNote];
      tone(SOUND_PIN, melody[thisNote]*8,noteDuration);
      int pauseBetweenNotes = noteDuration * 1.50;
      delay(pauseBetweenNotes);
      noTone(SOUND_PIN);
    }
}

Ticker displayer(scroll, 100);
Ticker speaker(playSound, 100);

void finishProgram(Tea* tea) {
  // setup
  resetFeedback();
  turnOn();
  Serial.println(tea->m_name);
  m_text_length = 35;
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.setCursor(3,0);
  lcd.print('Your tea ' + tea->m_name + ' is finish! Enjoy :)');
  pinMode(SOUND_PIN, OUTPUT);
  
  attachInterrupt(0, noButtonPressed, RISING);
  attachInterrupt(1, yesButtonPressed, RISING);
  
  displayer.start();
  speaker.start();
  
  int i=0;
  while(i<8 || !done) {
    displayer.update();
    speaker.update();
    i++;
  }
  turnOff();
}
