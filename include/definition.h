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

#define BTN_RESET (1 << PB0)
#define BTN_LED (1 << PB1)

typedef enum{
    DEFAULT,
    RGB,
    UART
} SYSTEM_MODE;

typedef enum{
    ON,
    OFF,
    TOGGLE
} LGT_STATE;

// typedef enum{
//     BTN_OFF = 0,
//     BTN_ON = 1,
//     BTN_RESET_ON = 2
// } BTN_STATE;

// typedef enum{
//     LOCK,
//     SELECT
// } ROTOR_SW_STATE;

// typedef enum{
//     DISABLE,
//     ENABLE,
//     TOGGLE
// } UART_STIR_STATE;

typedef struct{
    LGT_STATE led_red_st;
    LGT_STATE led_green_st;
    LGT_STATE led_blue_st;
    LGT_STATE led_white_st;
    LGT_STATE rgb_red_st;
    LGT_STATE rgb_green_st;
    LGT_STATE rgb_blue_st;
}Lightings_state;

typedef enum
{
    ROTOR_OFF = 0,
    ROTOR_RED = LED_RGB_RED,
    ROTOR_GREEN = LED_RGB_GREEN,
    ROTOR_BLUE = LED_RGB_BLUE,
    ROTOR_WHITE = 1,
    ROTOR_COLOR_COUNT = 5
} ROTOR_COLOR;


typedef enum 
{
    LED_ST_RED = LED_RED,
    LED_ST_GREEN = LED_GREEN,
    LED_ST_BLUE = LED_BLUE,
    LED_ST_WHITE = LED_WHITE,
    LED_ST_COUNT = 4
} LED_COLOR;

extern const LED_COLOR led_state_list[LED_ST_COUNT];

extern const ROTOR_COLOR rotor_color_list[ROTOR_COLOR_COUNT];

extern volatile  SYSTEM_MODE sys_mode;
extern volatile  uint8_t rotor_sw_select;
extern volatile  uint8_t btn_led;
extern volatile  uint8_t btn_reset;

extern Lightings_state lgt_defaultMode;

// extern volatile  Lightings_state leds_rgbMode;
// extern volatile  RgbGroup_state rgb_rgbMode;

extern volatile  Lightings_state lgt_uartMode;
extern volatile  Lightings_state lgt_rgbMode;
extern volatile  uint8_t rotor_state_idx;
extern volatile uint8_t rotor_sw_select;
extern volatile uint8_t btn_led_reset;

// extern uint8_t rgb_led_toggle;
// extern uint8_t led;
// extern uint8_t led_mask;
// extern uint8_t rotor_state_idx;
// extern uint8_t led_stir_mode;
// extern UART_STIR_STATE led_stir_choice;
// extern LED_COLOR led_stir_color;

#endif