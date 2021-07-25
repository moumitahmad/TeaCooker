#ifndef USERFEEDBACK_H
#define USERFEEDBACK_H

#include <Arduino.h>
#include "dataService.h"

void displayMessage(char* line1, char* line2, int dt);
bool askUser(char* line1, char* line2); // true=yes; false=no

void finishProgram(Tea* tea);

#endif
