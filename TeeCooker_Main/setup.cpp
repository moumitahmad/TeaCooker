#include "setup.h"
#include "userFeedback.h"

// -------- temperature sensor
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 4
#define DEVIATION 15

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature tempSensors(&oneWire);


// -------- color sensor
const int colorS0 = 8;
const int colorS1 = 9;
const int colorS2 = 11;
const int colorS3 = 12;
const int colorOut = 10;
int rot = 0;
int gruen = 0;
int blau = 0;


// -------- ligth sensor
#define LIGHTPIN A0
const int DARK_VALUE = 100;
bool setupIsFinished = false;
bool teabagIsThere = false;
bool cupIsThere = false;
bool waterTempIsCorrect = true;//false;
int dt = 500;


// -------- temp - servo motor
#include <Servo.h>
#define TEMP_SERVO_PIN 6
Servo servoTemp;
const int ANGLE_START = 180;
const int ANGLE_END = 70;

//----------- SETUP CUP --------
void checkForCup() {
  // check with Lightsensor
  // reads the input on analog pin (value between 0 and 1023)
  int analogValue = analogRead(LIGHTPIN);
  Serial.print("Analog reading = ");
  Serial.println(analogValue);
  if (analogValue == 0) {
    Serial.println("Cable is lose!");
    cupIsThere = false;
  } else if (analogValue < DARK_VALUE) {
    // cup is there
    Serial.println("Cup is there");
    cupIsThere = true;
  } else {
    Serial.println("Cup is NOT there");
    cupIsThere = false;
  }
}

double getWaterTemp() {
  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  Serial.println("Requesting temperatures...");
  tempSensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  // After we got the temperatures, we can print them here.
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  float tempC = tempSensors.getTempCByIndex(0);
  // Check if reading was successful
  if (tempC != DEVICE_DISCONNECTED_C) {
    Serial.print("Temperature: ");
    Serial.println(tempC);
    return tempC;
  } else {
    Serial.println("Error: Could not read temperature data");
    return;
  }
}

void waiting(double desiredWaterTemp) {
  while (true) {
    double temp = getWaterTemp();
    double tempDif = desiredWaterTemp - temp;
    if (tempDif <= DEVIATION) { // right temp
      displayMessage("The temperature", "is correct", 2000);
      waterTempIsCorrect = true;
      return;
    }
  }
}

void checkWater(double desiredWaterTemp, double roomTemp) {
  double temp = getWaterTemp();
  displayMessage("Temperature: ", temp, 1000);
  // temp is room temperature
  double tempDif = desiredWaterTemp - temp;
  if (tempDif <= DEVIATION) { // right temp
    displayMessage("The temperature", "is correct", 2000);
    waterTempIsCorrect = true;
  } else if (temp == roomTemp) { // roomtemperature -> there is no water inside the cup
    displayMessage("Cup is empthy", "Please fill it", 2000);
  } else if (tempDif <= DEVIATION + 3) {
    // temp is to high by "few" degrese
    // wait till the water cools down
    displayMessage("The temperature", "is a bit too hot", 2000);
    displayMessage("Please wait till", "it colds down", 2000);
    waiting(desiredWaterTemp);
  } else {
    // temp is far too high/too cold
    char* message = "This is too cold";
    if (desiredWaterTemp < temp)
      message = "This is too hot";
    // notify user to change the water
    displayMessage(message, "Please change it", 2000);
    displayMessage("It needs to be", temp, 2000);
  }
}

void bringTempSensor() {
  for (int angle = ANGLE_START; angle > ANGLE_END; angle--) {
    servoTemp.write(angle);
    Serial.println(angle);
    delay(15);
  }
}

void removeTempSensor() {
  for (int angle = ANGLE_END; angle < ANGLE_START; angle++) {
    servoTemp.write(angle);
    Serial.println(angle);
    delay(15);
  }
}

//-------SETUP TEABAG-------
void color() { // get color values and save in variables
  digitalWrite(colorS2, LOW);
  digitalWrite(colorS3, LOW);
  rot = pulseIn(colorOut, digitalRead(colorOut) == HIGH ? LOW : HIGH);
  digitalWrite(colorS3, HIGH);
  blau = pulseIn(colorOut, digitalRead(colorOut) == HIGH ? LOW : HIGH);
  digitalWrite(colorS2, HIGH);
  gruen = pulseIn(colorOut, digitalRead(colorOut) == HIGH ? LOW : HIGH);
}

int calculateTea() {
  Serial.print("blau: ");
  Serial.print(blau);
  Serial.print(", rot: ");
  Serial.print(rot);
  Serial.print(", grün: ");
  Serial.println(gruen);
  if (rot < blau && rot < gruen && rot < 20) {
    return 3;
  } else if (blau < rot && blau < gruen) {
    return 2;
  } else if (gruen < rot && gruen < blau) {
    return 1;
  } else { //Wenn keine Werte vorhanden sind..
    return 0;
  }
}

Tea* checkForTeabag() { // TODO: add color sensor test
  digitalWrite(colorS0, HIGH);
  digitalWrite(colorS1, HIGH);
  color();
  int tea = calculateTea();
  Serial.print("choose:");
  Serial.println(tea);
  if (tea == 0)
    return;
  Tea* choosenTea = getTea(tea);
  Serial.print("You choose the tea: ");
  Serial.println(choosenTea->m_name);
  char* teaSpecs = 'The tea will be brewed for ' + (char)choosenTea->m_brewingTime + ' min in water with a temperature of ' + (char)choosenTea->m_waterTemp + ' degrees.';
  displayMessage("Your choice is:", choosenTea->m_name, 3000);
  Serial.println(teaSpecs);
  if (askUser("Are you happy", "with the choice?")) {
    teabagIsThere = true;
    Serial.println("Teabag there");
    return choosenTea;
  } else {
    teabagIsThere = false;
    Serial.println("Teabag NOT there");
    return nullptr;
  }
}

Tea* startProgram() {
  Tea* tea;
  displayMessage("Let's brew some", "tea! :)", 2000);
  while (!askUser("Do you want to", "start?")) {}
  // check for teebag
  // setup color sensor
  pinMode(colorS0, OUTPUT);
  pinMode(colorS1, OUTPUT);
  pinMode(colorS2, OUTPUT);
  pinMode(colorS3, OUTPUT);
  pinMode(colorOut, INPUT);
  digitalWrite(colorS0, LOW);
  digitalWrite(colorS1, LOW);
  while (!teabagIsThere) {
    displayMessage("Scan a tea...", "", 500);
    tea = checkForTeabag();
    delay(dt);
  } // teabag is ready
  // TODO: switch sensor off
  digitalWrite(colorS0, LOW);
  digitalWrite(colorS1, LOW);

  // check for cup
  displayMessage("Wainting for a", "cup...", 500);
  while (!cupIsThere) {
    checkForCup();
    delay(dt);
  } // cup is ready
  displayMessage("The cup is ready", "", 800);

  // check water temp
  // setup
  /*tempSensors.begin();
  double roomTemp = getWaterTemp();
  Serial.println("Room-Temperature: " + (char)roomTemp);
  displayMessage("Checking the", "water temp...", 800);
  // lower temperature sensor into cup
  servoTemp.attach(TEMP_SERVO_PIN);
  bringTempSensor();
  while (!waterTempIsCorrect) {
    Serial.println("wainting for correct water temp...");
    checkWater(tea->m_waterTemp, roomTemp);
    delay(dt);
  } // cup is ready
  removeTempSensor();*/

  return tea;
}
