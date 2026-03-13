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

typedef enum{
    DEFAULT,
    RGB,
    UART
} SYSTEM_MODE;

typedef enum{
    ON = 0,
    OFF = 1,
    TOGGLE = 2
} LGT_STATE;

// typedef enum{
//     ON,
//     OFF,
//     RESET
// } BTN_STATE;

typedef enum{
    LOCK,
    SELECT
} ROTOR_SW_STATE;

// typedef enum{
//     DISABLE,
//     ENABLE,
//     TOGGLE
// } UART_STIR_STATE;

typedef struct{
    LGT_STATE red_st;
    LGT_STATE green_st;
    LGT_STATE blue_st;
    LGT_STATE white_st;
}LedGroup_state;

typedef struct{
    LGT_STATE red_st;
    LGT_STATE green_st;
    LGT_STATE blue_st;
}RgbGroup_state;

typedef enum
{
    RGB_OFF = 0,
    RGB_RED = LED_RGB_RED,
    RGB_GREEN = LED_RGB_GREEN,
    RGB_BLUE = LED_RGB_BLUE,
    RGB_WHITE = 1,
    RGB_COUNT = 5
} RGB_COLOR;

typedef enum 
{
    LED_ST_RED = LED_RED,
    LED_ST_GREEN = LED_GREEN,
    LED_ST_BLUE = LED_BLUE,
    LED_ST_WHITE = LED_WHITE,
    LED_ST_COUNT = 4
} LED_COLOR;

extern volatile  SYSTEM_MODE sys_mode;

extern LedGroup_state leds_defaultMode;
extern RgbGroup_state rgb_defaultMode;

// extern volatile  LedGroup_state leds_rgbMode;
// extern volatile  RgbGroup_state rgb_rgbMode;

extern volatile  LedGroup_state leds_uartMode;
extern volatile  RgbGroup_state rgb_uartMode;

// extern uint8_t rgb_led_toggle;
// extern uint8_t led;
// extern uint8_t led_mask;
// extern uint8_t rotor_state_idx;
// extern uint8_t led_stir_mode;
// extern UART_STIR_STATE led_stir_choice;
// extern LED_COLOR led_stir_color;

#endif