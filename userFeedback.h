#ifndef USERFEEDBACK_H
#define USERFEEDBACK_H

#include <Arduino.h>
#include "dataService.h"

void displayMessage(char* message, int text_length);
bool askUser(char* message, int text_length); // true=yes; false=no

void finishProgram(Tea* tea);

#endif
