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

#define ROTOR_COLOR_COUNT 5
#define _OFF 0
#define _RED 1
#define _GREEN 2
#define _BLUE 3
#define _WHITE 4
#define _rgb_btn_OFF 0
#define _rgb_btn_ON 1
#define _rgb_btn_BLINK 2
#define _rgb_btn_X 3

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

extern volatile uint8_t btn_led_reset;
extern volatile uint8_t btn_led_isPressed;

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

extern volatile FLAG led_red_flag_uart;
extern volatile FLAG led_green_flag_uart;
extern volatile FLAG led_blue_flag_uart;
extern volatile FLAG led_white_flag_uart;

extern volatile FLAG led_red_flag_rgb;
extern volatile FLAG led_green_flag_rgb;
extern volatile FLAG led_blue_flag_rgb;
extern volatile FLAG led_white_flag_rgb;

extern volatile FLAG blinkstadiet_flag;
extern volatile uint8_t blinkstadiet_count;
extern volatile FLAG current_flag_states[4];



#endif