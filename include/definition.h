#ifndef DEFINITION_H
#define DEFINITION_H

#include <avr/io.h>
#include <stdint.h>

#define LED_WHITE (1 << PB2)
#define LED_RED (1 << PB5)
#define LED_GREEN (1 << PB4)
#define LED_BLUE (1 << PB3)
#define LED_MASK (LED_RED | LED_GREEN | LED_BLUE | LED_WHITE)

#define LED_RGB_RED (1 << PD7)
#define LED_RGB_BLUE (1 << PD5)
#define LED_RGB_GREEN (1 << PD6)
#define LED_RGB_MASK (LED_RGB_RED | LED_RGB_GREEN | LED_RGB_BLUE)

#define ROTOR_CLK (1 << PD2)
#define ROTOR_DT (1 << PD3)
#define ROTOR_SW (1 << PD4)

#define BTN_RESET (1 << PB0)
#define BTN_LED (1 << PB1)

#define _OFF 0
#define _RED 1
#define _GREEN 2
#define _BLUE 3
#define _WHITE 4

typedef enum{
    DEFAULT,
    RGB,
    UART
} SYSTEM_MODE;

typedef enum{
    ON = 1,
    OFF = 0,
    BLINK = 2
} FLAG;

// typedef struct{
//     uint8_t led;
//     FLAG state;
// } Lgt_FLAG;

// typedef struct{
//     Lgt_FLAG red;
//     Lgt_FLAG blue;
//     Lgt_FLAG gree;
//     Lgt_FLAG white;
//     Lgt_FLAG rgb_red;
//     Lgt_FLAG rgb_blue;
//     Lgt_FLAG rgb_green;
// }Lightings_state;

// typedef enum
// {
//     ROTOR_OFF = 0,
//     ROTOR_RED = LED_RGB_RED,
//     ROTOR_GREEN = LED_RGB_GREEN,
//     ROTOR_BLUE = LED_RGB_BLUE,
//     ROTOR_WHITE = LED_RGB_RED | LED_RGB_GREEN | LED_RGB_BLUE,
//     ROTOR_COLOR_COUNT = 5
// } ROTOR_COLOR;

// typedef enum 
// {
//     LED_ST_RED = LED_RED,
//     LED_ST_GREEN = LED_GREEN,
//     LED_ST_BLUE = LED_BLUE,
//     LED_ST_WHITE = LED_WHITE,
//     LED_ST_COUNT = 4
// } LED_COLOR;

// extern const LED_COLOR led_state_list[LED_ST_COUNT];
// extern const ROTOR_COLOR rotor_color_list[ROTOR_COLOR_COUNT];


#define ROTOR_COLOR_COUNT 5

// extern Lightings_state lgt_defaultMode;
// extern volatile  Lightings_state lgt_uartMode;
// extern volatile  Lightings_state lgt_rgbMode;
// extern volatile uint8_t led_state_idx;
// extern volatile uint8_t btn_led_reset;
// extern volatile uint8_t led_blinkstadiet;

extern volatile SYSTEM_MODE sys_mode;
extern volatile  uint8_t rotor_state_idx;
extern volatile uint8_t rotor_sw_select;

extern volatile uint8_t blink_state;
extern volatile uint8_t blink_state_rgb;
extern volatile uint8_t output_default;
extern volatile uint8_t output_uart;
extern volatile uint8_t output_led_rgb;
extern volatile uint8_t output_rgb_rgb;

extern volatile uint32_t extraTime_BlinkStage;

#define LIGHTS_MAX 7

// extern volatile Lightings_state flag_state_uart;
// extern volatile Lightings_state flag_state_rgb;

extern volatile FLAG led_red_flag_uart;
extern volatile FLAG led_green_flag_uart;
extern volatile FLAG led_blue_flag_uart;
extern volatile FLAG led_white_flag_uart;

// extern volatile char *input;



#endif