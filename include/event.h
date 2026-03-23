#ifndef EVENT_H
#define EVENT_H

#include <avr/io.h>
#include "../include/definition.h"

void checkFlag_UART(uint8_t mask, FLAG *f);

void flag_manager();

// void setBit_LED(LGT_STATE st, LED_COLOR led);

// void setBit_RGB(LGT_STATE st, ROTOR_COLOR rgb_x);

// void updateStructLedGroup(Lightings_state *st, LGT_STATE red, LGT_STATE green, LGT_STATE blue, LGT_STATE white);

// void updateStructRGBGroup(Lightings_state *st, LGT_STATE red, LGT_STATE green, LGT_STATE blue);

// void updateLightingBits(Lightings_state *st);

// void updateLEDbits_blinkstadiet();

// void updateLEDbits_RGBmode_BtnIsPressed(uint8_t btn_led_rest);

#endif