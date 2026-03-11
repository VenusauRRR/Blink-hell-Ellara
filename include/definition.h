#ifndef DEFINITION_H
#define DEFINITION_H

#include <stdint.h>

#define LED_WHITE (1 << PB2)
#define LED_RED (1 << PB5)
#define LED_GREEN (1 << PB4)
#define LED_BLUE (1 << PB3)

#define LED_RGB_RED (1 << PD7)
#define LED_RGB_BLUE (1 << PD5)
#define LED_RGB_GREEN (1 << PD6)
#define LED_RGB_MASK (LED_RGB_RED | LED_RGB_GREEN | LED_RGB_BLUE)

#define ROTOR_CLK (1 << PD2)
#define ROTOR_DT (1 << PD3)
#define ROTOR_SW (1 << PD4)

#define BTN_RED (1 << PB0)
#define BTN_GREEN (1 << PB1)

typedef enum 
{
    ROTOR_ST_OFF = 0,
    ROTOR_ST_RED = LED_RGB_RED,
    ROTOR_ST_GREEN = LED_RGB_GREEN,
    ROTOR_ST_BLUE = LED_RGB_BLUE,
    ROTOR_ST_ALL = LED_RGB_MASK,
    ROTOR_ST_COUNT = 5
} ROTOR_STATE;

typedef enum 
{
    LED_ST_RED = LED_RED,
    LED_ST_GREEN = LED_GREEN,
    LED_ST_BLUE = LED_BLUE,
    LED_ST_WHITE = LED_WHITE,
    LED_ST_COUNT = 4
} LED_STATE;

typedef enum{
    DISABLE,
    ENABLE,
    TOGGLE
} LED_STIR_STATE;


extern uint8_t rgb_led_toggle;
extern uint8_t led;
extern uint8_t led_mask;
extern uint8_t rotor_state_idx;
extern uint8_t led_stir_mode;
extern LED_STIR_STATE led_stir_choice;
extern LED_STATE led_stir_color;

#endif