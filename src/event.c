#include "../include/event.h"

#include <avr/io.h>
#include "../include/definition.h"
#include "../include/uart.h"

static uint8_t selectedLEDMask_rgb;
static uint8_t selectedRGBMask_rgb;
static FLAG *selectedFlag_rgb;

volatile FLAG led_red_flag_rgb = BLINK;
volatile FLAG led_green_flag_rgb = BLINK;
volatile FLAG led_blue_flag_rgb = BLINK;
volatile FLAG led_white_flag_rgb = BLINK;

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

void updateRGBColor()
{
    switch (rotor_state_idx)
    {
    case _rgb_OFF:
        output_rgb_rgb &= ~LED_RGB_MASK;
        // stadiet
        break;
    case _rgb_RED:
        output_rgb_rgb = (output_rgb_rgb & ~LED_RGB_MASK) | LED_RGB_RED;
        selectedLEDMask_rgb = LED_RED;
        selectedRGBMask_rgb = LED_RGB_RED;
        break;
    case _rgb_GREEN:
        output_rgb_rgb = (output_rgb_rgb & ~LED_RGB_MASK) | LED_RGB_GREEN;
        selectedLEDMask_rgb = LED_GREEN;
        selectedRGBMask_rgb = LED_RGB_GREEN;

        break;
    case _rgb_BLUE:
        output_rgb_rgb = (output_rgb_rgb & ~LED_RGB_MASK) | LED_RGB_BLUE;
        selectedLEDMask_rgb = LED_BLUE;
        selectedRGBMask_rgb = LED_RGB_BLUE;
        break;
    case _rgb_WHITE:
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

void updateLEDColorFromRGB_whenRotorSwitchIsPressed()
{
    if (!rotor_sw_select)
    {
        return;
    }

    switch (rotor_state_idx)
    {
    case _rgb_OFF:
        output_rgb_rgb &= ~LED_RGB_MASK;
        // stadiet
        break;
    case _rgb_RED:
        selectedFlag_rgb = &led_red_flag_rgb;
        break;
    case _rgb_GREEN:
        selectedFlag_rgb = &led_green_flag_rgb;
        break;
    case _rgb_BLUE:
        selectedFlag_rgb = &led_blue_flag_rgb;
        break;
    case _rgb_WHITE:
        selectedFlag_rgb = &led_white_flag_rgb;
        break;
    default:
        break;
    }
}

void updateLEDColorFromRGB_whenBtnIsPressed(){
    if (btn_led_reset == _rgb_btn_X){
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
        btn_led_reset = _rgb_btn_X;
        checkFlag(LED_RED, &output_uart, &blink_state, &led_red_flag_uart);
        checkFlag(LED_GREEN, &output_uart, &blink_state, &led_green_flag_uart);
        checkFlag(LED_BLUE, &output_uart, &blink_state, &led_blue_flag_uart);
        checkFlag(LED_WHITE, &output_uart, &blink_state, &led_white_flag_uart);
        break;
    case RGB:
        updateRGBColor();
        selectRGBcolor();
        updateLEDColorFromRGB_whenRotorSwitchIsPressed();
        updateLEDColorFromRGB_whenBtnIsPressed();
        checkFlag(LED_RED, &output_led_rgb, &blink_state, &led_red_flag_rgb);
        checkFlag(LED_GREEN, &output_led_rgb, &blink_state, &led_green_flag_rgb);
        checkFlag(LED_BLUE, &output_led_rgb, &blink_state, &led_blue_flag_rgb);
        checkFlag(LED_WHITE, &output_led_rgb, &blink_state, &led_white_flag_rgb);
        break;
    default:
        break;
    }
}