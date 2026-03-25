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
        // selectedFlag_rgb = &led_red_flag_rgb;
        break;
    case _rgb_GREEN:
        output_rgb_rgb = (output_rgb_rgb & ~LED_RGB_MASK) | LED_RGB_GREEN;
        selectedLEDMask_rgb = LED_GREEN;
        selectedRGBMask_rgb = LED_RGB_GREEN;
        // selectedFlag_rgb = &led_green_flag_rgb;

        break;
    case _rgb_BLUE:
        output_rgb_rgb = (output_rgb_rgb & ~LED_RGB_MASK) | LED_RGB_BLUE;
        selectedLEDMask_rgb = LED_BLUE;
        selectedRGBMask_rgb = LED_RGB_BLUE;
        // selectedFlag_rgb = &led_blue_flag_rgb;
        break;
    case _rgb_WHITE:
        output_rgb_rgb |= LED_RGB_MASK;
        selectedLEDMask_rgb = LED_WHITE;
        selectedRGBMask_rgb = LED_RGB_MASK;
        // selectedFlag_rgb = &led_white_flag_rgb;
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
    // uint8_t currentState = (output_led_rgb & LED_MASK);
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
    if (btn_led_reset == 4){
        return;
    }
    FLAG temp;
    if (btn_led_reset == 0)
    {
        temp = OFF;
    }
    else if (btn_led_reset == 1)
    {
        temp = ON;
    }
    else if (btn_led_reset == 2)
    {
        temp = BLINK;
    }

    // uart_print("before: ");
    // uart_print_uint16(*selectedFlag_rgb);
    // *selectedFlag_rgb = temp;
    // uart_print("\r\n");
    // uart_print("after: ");
    // uart_print_uint16(*selectedFlag_rgb);
    // uart_print("\r\n");
    *selectedFlag_rgb = temp;
    // if (btn_led_reset>=0 && btn_led_reset <4){
        // output_led_rgb &= ~selectedLEDMask_rgb;
    //         uart_print("red: ");
    // uart_print_uint16(led_red_flag_rgb);
    // uart_print(",green: ");
    // uart_print_uint16(led_green_flag_rgb);
    // uart_print(",blue: ");
    // uart_print_uint16(led_blue_flag_rgb);
    // uart_print(",white: ");
    // uart_print_uint16(led_white_flag_rgb);
    // temp = BLINK;
    // btn_led_reset = 4;
    // }
}

void flag_manager()
{
    switch (sys_mode)
    {
    case UART:
        // uart_print("red flag: ");'
        // uart_print_uint16(led_red'_flag);
        btn_led_reset = -1;
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
        // updateLEDColorFromRGB_whenBtnIsPressed();
        break;
    default:
        break;
    }
}

// volatile uint8_t led_state_idx = 0;

// const LED_COLOR led_state_list[LED_ST_COUNT] = {
//     LED_ST_RED,
//     LED_ST_GREEN,
//     LED_ST_BLUE,
//     LED_ST_WHITE};

// const ROTOR_COLOR rotor_color_list[ROTOR_COLOR_COUNT] = {
//     ROTOR_OFF,
//     ROTOR_RED,
//     ROTOR_GREEN,
//     ROTOR_BLUE,
//     ROTOR_WHITE,
//     ROTOR_COLOR_COUNT};

// void setBit_LED(LGT_STATE st, LED_COLOR led)
// {
//     switch (st)
//     {
//     case ON:
//         PORTB |= led;
//         break;
//     case OFF:
//         PORTB &= ~led;
//         break;
//     case TOGGLE:
//         PORTB ^= led;
//         break;
//     default:
//         break;
//     }
// }

// void setBit_RGB(LGT_STATE st, ROTOR_COLOR rgb_x)
// {
//     switch (st)
//     {
//     case ON:
//         PORTD |= rgb_x;
//         break;
//     case OFF:
//         PORTD &= ~rgb_x;
//         break;
//     case TOGGLE:
//         PORTD ^= rgb_x;
//         break;
//     default:
//         break;
//     }
// }

// void updateStructLedGroup(Lightings_state *st, LGT_STATE red, LGT_STATE green, LGT_STATE blue, LGT_STATE white)
// {
//     st->led_red_st = red;
//     st->led_green_st = green;
//     st->led_blue_st = blue;
//     st->led_white_st = white;
// }

// void updateStructRGBGroup(Lightings_state *st, LGT_STATE red, LGT_STATE green, LGT_STATE blue)
// {
//     st->rgb_red_st = red;
//     st->rgb_green_st = green;
//     st->rgb_blue_st = blue;
// }

// void updateLightingBits(Lightings_state *st)
// {
//     setBit_LED(st->led_red_st, LED_ST_RED);
//     setBit_LED(st->led_blue_st, LED_ST_BLUE);
//     setBit_LED(st->led_green_st, LED_ST_GREEN);
//     setBit_LED(st->led_white_st, LED_ST_WHITE);
//     setBit_RGB(st->rgb_red_st, ROTOR_RED);
//     setBit_RGB(st->rgb_blue_st, ROTOR_BLUE);
//     setBit_RGB(st->rgb_green_st, ROTOR_GREEN);
// }

// void updateLEDbits_blinkstadiet()
// {
//     LED_COLOR c = led_state_list[led_state_idx];
//     switch (c)
//     {
//     case LED_RED:
//         updateStructLedGroup(&lgt_rgbMode, ON, OFF, OFF, OFF);
//         break;
//     case LED_GREEN:
//         updateStructLedGroup(&lgt_rgbMode, OFF, ON, OFF, OFF);
//         break;
//     case LED_BLUE:
//         updateStructLedGroup(&lgt_rgbMode, OFF, OFF, ON, OFF);
//         break;
//     case LED_WHITE:
//         updateStructLedGroup(&lgt_rgbMode, OFF, OFF, OFF, ON);
//         break;
//     default:
//         break;
//     }
// }

// void updateLEDbits_RGBmode_BtnIsPressed(uint8_t btn_led_rest)
// {
//     LGT_STATE st;
//     ROTOR_COLOR c = rotor_color_list[rotor_state_idx];
//     switch (btn_led_rest)
//     {
//     case 0:
//         st = OFF;
//         break;
//     case 1:
//         st = ON;
//         break;
//     case 2:
//         st = TOGGLE;
//         break;
//     default:
//         break;
//     }
//     switch (c)
//     {
//     case ROTOR_RED:
//         lgt_rgbMode.led_red_st = st;
//         break;
//     case ROTOR_GREEN:
//         lgt_rgbMode.led_green_st = st;
//         break;
//     case ROTOR_BLUE:
//         lgt_rgbMode.led_blue_st = st;
//         break;
//     case ROTOR_WHITE:
//         lgt_rgbMode.led_white_st = st;
//         break;
//     default:
//         break;
//     }
// }