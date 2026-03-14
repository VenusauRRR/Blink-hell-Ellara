#ifndef ROTOR_H
#define ROTOR_H

#include <avr/io.h>
#include "../include/definition.h"
// #include "../include/millis.h"
// #include "../include/uart.h"

void updateRGBcolor_switchIsPressed();

void updateRGBcolor_switchIsNotPressed();

void rotor_init(void);

#endif