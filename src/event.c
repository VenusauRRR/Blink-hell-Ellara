#include "../include/event.h"

#include <avr/io.h>
#include "../include/definition.h"
#include "../include/uart.h"

static uint8_t selectedLEDMask_rgb;
static uint8_t selectedRGBMask_rgb;
static FLAG *selectedFlag_rgb;

volatile FLAG current_flag_states[4];

volatile FLAG led_red_flag_rgb = BLINK;
volatile FLAG led_green_flag_rgb = BLINK;
volatile FLAG led_blue_flag_rgb = BLINK;
volatile FLAG led_white_flag_rgb = BLINK;

volatile uint8_t blinkstadiet_count = 0;
volatile FLAG blinkstadiet_flag = OFF;
static FLAG led_red_flag_blinkstadiet = OFF;
static FLAG led_green_flag_blinkstadiet = OFF;
static FLAG led_blue_flag_blinkstadiet = OFF;
static FLAG led_white_flag_blinkstadiet = OFF;

void readLedFlagStates()
{
    switch (sys_mode)
    {
    case UART:
        current_flag_states[0] = led_red_flag_uart;
        current_flag_states[1] = led_green_flag_uart;
        current_flag_states[2] = led_blue_flag_uart;
        current_flag_states[3] = led_white_flag_uart;
        break;
    case RGB:
        if (blinkstadiet_flag == ON)
        {
            current_flag_states[0] = led_red_flag_blinkstadiet;
            current_flag_states[1] = led_green_flag_blinkstadiet;
            current_flag_states[2] = led_blue_flag_blinkstadiet;
            current_flag_states[3] = led_white_flag_blinkstadiet;
        }
        else
        {
            current_flag_states[0] = led_red_flag_rgb;
            current_flag_states[1] = led_green_flag_rgb;
            current_flag_states[2] = led_blue_flag_rgb;
            current_flag_states[3] = led_white_flag_rgb;
        }
        break;
    default:
        break;
    }
}

void checkFlag(uint8_t mask, uint8_t *output, uint8_t *blinkState, FLAG *f)
{
    switch (*f)
    {
    case OFF:
        (*output) &= ~(mask);
        break;
    case ON:
        (*output) |= (mask);
        break;
    case BLINK:
        (*output) = ((*output) & ~(mask)) | ((*blinkState) & (mask));
        break;
    default:
        break;
    }
}

void checkFlagAndDisplayLedColors()
{
    readLedFlagStates();
    uint8_t *temp;
    switch (sys_mode)
    {
    case UART:
        temp = &output_uart;
        break;
    case RGB:
        temp = &output_led_rgb;
        break;
    default:
        break;
    }
    checkFlag(LED_RED, temp, &blink_state, &current_flag_states[0]);
    checkFlag(LED_GREEN, temp, &blink_state, &current_flag_states[1]);
    checkFlag(LED_BLUE, temp, &blink_state, &current_flag_states[2]);
    checkFlag(LED_WHITE, temp, &blink_state, &current_flag_states[3]);
}

void updateRGBColor()
{
    switch (rotor_state_idx)
    {
    case _OFF:
        output_rgb_rgb &= ~LED_RGB_MASK;
        break;
    case _RED:
        output_rgb_rgb = (output_rgb_rgb & ~LED_RGB_MASK) | LED_RGB_RED;
        selectedLEDMask_rgb = LED_RED;
        selectedRGBMask_rgb = LED_RGB_RED;
        break;
    case _GREEN:
        output_rgb_rgb = (output_rgb_rgb & ~LED_RGB_MASK) | LED_RGB_GREEN;
        selectedLEDMask_rgb = LED_GREEN;
        selectedRGBMask_rgb = LED_RGB_GREEN;

        break;
    case _BLUE:
        output_rgb_rgb = (output_rgb_rgb & ~LED_RGB_MASK) | LED_RGB_BLUE;
        selectedLEDMask_rgb = LED_BLUE;
        selectedRGBMask_rgb = LED_RGB_BLUE;
        break;
    case _WHITE:
        output_rgb_rgb |= LED_RGB_MASK;
        selectedLEDMask_rgb = LED_WHITE;
        selectedRGBMask_rgb = LED_RGB_MASK;
        break;
    default:
        break;
    }
}

void selectRGBcolor()
{
    if (rotor_sw_select)
    {
        output_rgb_rgb = (output_rgb_rgb & ~LED_RGB_MASK) | (blink_state_rgb & selectedRGBMask_rgb);
    }
}

void blinkstadiet()
{
    if (blinkstadiet_flag == OFF)
    {
        return;
    }
    switch (blinkstadiet_count +1 )
    {
    case _WHITE:
        led_red_flag_blinkstadiet = OFF;
        led_green_flag_blinkstadiet = OFF;
        led_blue_flag_blinkstadiet = OFF;
        led_white_flag_blinkstadiet = ON;
        break;
    case _RED:
        led_red_flag_blinkstadiet = ON;
        led_green_flag_blinkstadiet = OFF;
        led_blue_flag_blinkstadiet = OFF;
        led_white_flag_blinkstadiet = OFF;
        break;
    case _GREEN:
        led_red_flag_blinkstadiet = OFF;
        led_green_flag_blinkstadiet = ON;
        led_blue_flag_blinkstadiet = OFF;
        led_white_flag_blinkstadiet = OFF;
        break;
    case _BLUE:
        led_red_flag_blinkstadiet = OFF;
        led_green_flag_blinkstadiet = OFF;
        led_blue_flag_blinkstadiet = ON;
        led_white_flag_blinkstadiet = OFF;
        break;

    default:
        break;
    }
}

void updateLEDColorFromRGB_whenRotorSwitchIsPressed()
{
    if (!rotor_sw_select)
    {
        return;
    }
    switch (rotor_state_idx)
    {
    case _OFF:
        blinkstadiet_flag = ON;
        readLedFlagStates();
        blinkstadiet();
        break;
    case _RED:
        selectedFlag_rgb = &led_red_flag_rgb;
        break;
    case _GREEN:
        selectedFlag_rgb = &led_green_flag_rgb;
        break;
    case _BLUE:
        selectedFlag_rgb = &led_blue_flag_rgb;
        break;
    case _WHITE:
        selectedFlag_rgb = &led_white_flag_rgb;
        break;
    default:
        break;
    }
}

void updateLEDColorFromRGB_whenBtnIsPressed()
{
    if (btn_led_reset == _rgb_btn_init)
    {
        return;
    }
    FLAG temp;
    if (btn_led_reset == _rgb_btn_OFF)
    {
        temp = OFF;
    }
    else if (btn_led_reset == _rgb_btn_ON)
    {
        temp = ON;
    }
    else if (btn_led_reset == _rgb_btn_BLINK)
    {
        temp = BLINK;
    }
    *selectedFlag_rgb = temp;
}

void flag_manager()
{
    switch (sys_mode)
    {
    case UART:
        blinkstadiet_flag = OFF;
        btn_led_reset = _rgb_btn_init;
        checkFlagAndDisplayLedColors();
        break;
    case RGB:
        updateRGBColor();
        selectRGBcolor();
        updateLEDColorFromRGB_whenRotorSwitchIsPressed();
        updateLEDColorFromRGB_whenBtnIsPressed();
        checkFlagAndDisplayLedColors();
        break;
    default:
        break;
    }
}