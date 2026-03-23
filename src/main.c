#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

#include "../include/millis.h"
#include "../include/adc.h"
#include "../include/uart.h"
#include "../include/definition.h"
#include "../include/rotor.h"
#include "../include/event.h"

volatile SYSTEM_MODE sys_mode;

Lightings_state lgt_defaultMode;

volatile Lightings_state lgt_uartMode;

volatile Lightings_state lgt_rgbMode;

volatile uint8_t rotor_sw_select = 0;
volatile uint8_t btn_led_reset = 2;

const unsigned long interval = 250;

volatile uint8_t blink_state = 0;
volatile uint8_t uart_output;
volatile uint8_t rgb_led_output;
volatile uint8_t rgb_output;
volatile uint8_t red_flag = 0;
volatile uint8_t rgb_flag = 0;
volatile uint8_t green_flag_default = 0;

int main(void)
{
    // adc_init(1, 128);
    uart_init(9600);
    timer_init();
    rotor_init();
    sei();

    sys_mode = DEFAULT;

    // DDRB |= (LED_BLUE | LED_GREEN | LED_RED | LED_WHITE);
    // DDRD |= (LED_RGB_RED | LED_RGB_GREEN | LED_RGB_BLUE);
    // DDRD |= BTN_LED;
    // DDRD |= BTN_RESET;
    // DDRD &= ~(ROTOR_SW); // set as input
    // PORTD |= ROTOR_SW;

    DDRB |= LED_MASK;               // set as input
    PORTB |= LED_MASK;              // set as ON
    DDRB &= ~BTN_LED;               // set btn as input
    DDRB &= ~BTN_RESET;             // set btn as input
    PORTB |= (BTN_LED | BTN_RESET); // enable internal pull up
    DDRD &= ~(ROTOR_SW);            // set as input
    PORTD |= ROTOR_SW;
    DDRD |= (LED_RGB_RED | LED_RGB_GREEN | LED_RGB_BLUE);

    unsigned long currentMilli = milliSec_get();
    unsigned long previousMilli = 0;

    // updateStructLedGroup(&lgt_defaultMode, TOGGLE, TOGGLE, TOGGLE, TOGGLE);
    // updateStructRGBGroup(&lgt_defaultMode, OFF, OFF, OFF);

    // updateStructLedGroup(&lgt_uartMode, TOGGLE, TOGGLE, TOGGLE, TOGGLE);
    // updateStructRGBGroup(&lgt_uartMode, OFF, OFF, OFF);

    // updateStructLedGroup(&lgt_rgbMode, TOGGLE, TOGGLE, TOGGLE, TOGGLE);
    // updateStructRGBGroup(&lgt_rgbMode, OFF, OFF, OFF);

    // updateLightingBits(&lgt_defaultMode);

    // // Debounce
    unsigned long debounce_time = 20;
    unsigned long prev_sw_debounce = 0;
    unsigned long prev_btn_led_debounce = 0;
    unsigned long prev_btn_reset_debounce = 0;
    uint8_t prev_sw_state = 0; // Knappen hög --> 5V
    uint8_t true_sw_state = 1;
    // uint8_t prev_btn_led_state = 0; // Knappen hög --> 5V
    // uint8_t true_btn_led_state = 1;
    // uint8_t prev_btn_reset_state = 0; // Knappen hög --> 5V
    // uint8_t true_btn_reset_state = 1;

    uint32_t extraTime_BlinkStage = 0;

    uint8_t press_btn_led;
    uint8_t press_btn_reset;

    while (1)
    {
        uint16_t potentioReading_A0 = adc_read(0);
        uint16_t potentioReading_A1 = adc_read(1);
        uint32_t convertedPotentioReading_A0 = (uint32_t)potentioReading_A0 * 255 / 1023;

        currentMilli = milliSec_get();
        if (currentMilli - previousMilli >= interval + milliSec_addTime(convertedPotentioReading_A0) + milliSec_addTime(extraTime_BlinkStage))
        {
            previousMilli = currentMilli;

            blink_state ^= LED_MASK;

            // PORTB |= ((blink_state ^ LED_MASK) & LED_MASK);

            // switch (sys_mode)
            // {
            // case DEFAULT:
            // uart_print("default mode");
            // updateLightingBits(&lgt_defaultMode);
            // break;
            // case RGB:
            // uart_print("rgb mode");
            // updateLightingBits_RGB(&lgt_rgbMode);
            // break;
            // case UART:
            // uart_print("uart mode");
            //     updateLightingBits(&lgt_uartMode);
            //     break;
            // default:
            //     break;
            // }

            // if (rotor_sw_select)
            // {
            //     updateRGBcolor_switchIsPressed();
            //     if (led_blinkstadiet == 1)
            //     {
            //         extraTime_BlinkStage = (uint32_t)potentioReading_A1 * 255 / 1023;
            //         updateLEDbits_blinkstadiet();
            //         updateLightingBits(&lgt_rgbMode);
            //         led_state_idx = (led_state_idx + 1) % 4;
            //     }
            // }
            // else
            // {
            //     led_blinkstadiet = 0;
            //     extraTime_BlinkStage = 0;
            //     // updateStructLedGroup(&lgt_rgbMode, OFF, OFF, OFF, OFF);
            //     // updateStructLedGroup(&lgt_rgbMode, TOGGLE, TOGGLE, TOGGLE, TOGGLE);
            //     updateRGBcolor_switchIsNotPressed();
            // }
        }
        uint8_t output = blink_state;
        uart_output = blink_state;
        rgb_led_output = blink_state;

        press_btn_led = (PINB & BTN_LED) == 0;
        press_btn_reset = (PINB & BTN_RESET) == 0;
        if (press_btn_led)
        { // if btn_led is pressed
            // uart_print("btn led is pressed.");
            green_flag_default = 1;
            sys_mode = DEFAULT;
            // output &= ~LED_RED; // set led red OFF
            // uart_print(" red shall OFF");
            // PORTB &= ~LED_RED;
        }
        if (press_btn_reset)
        {
            green_flag_default = 2;
            // output |= LED_MASK;
            // PORTB = (PORTB & ~LED_MASK) | (output & LED_MASK);
        }

        if (red_flag == 1)
        {

            uart_output &= ~LED_RED;
        }
        else if (red_flag == 2)
        {
            uart_output |= LED_RED;
        }
        else
        {
            uart_output = ((blink_state & ~LED_RED) | (blink_state & LED_RED));
        }

        if (green_flag_default == 1)
        {
            output &= ~LED_GREEN;
        }
        else if (green_flag_default == 2)
        {
            output |= ((output & ~LED_GREEN) | (blink_state & LED_GREEN));
        }
        if (sys_mode == RGB)
        {
            switch (rgb_flag)
            {
            case 0:
                rgb_led_output &= ~LED_MASK;
                rgb_output &= ~LED_RGB_MASK;
                break;
            case 1:
                rgb_led_output = ((rgb_led_output & ~LED_MASK) | LED_RED);
                rgb_output = ((rgb_output & ~LED_RGB_MASK) | LED_RGB_RED);
                break;
            case 2:
                rgb_led_output = ((rgb_led_output & ~LED_MASK) | LED_GREEN);
                rgb_output = ((rgb_output & ~LED_RGB_MASK) | LED_RGB_GREEN);
                break;
            case 3:
                rgb_led_output = ((rgb_led_output & ~LED_MASK)| LED_BLUE);
                rgb_output = ((rgb_output & ~LED_RGB_MASK) | LED_RGB_BLUE);
                break;
            case 4:
                rgb_led_output = ((rgb_led_output & ~LED_MASK) | LED_WHITE);
                rgb_output |= LED_RGB_MASK;
                break;
            default:
                break;
            }
        }

        if (sys_mode == UART)
        {
            PORTB = (PORTB & ~LED_MASK) | (uart_output & LED_MASK);
        }
        else if (sys_mode == RGB)
        {
            PORTB = (PORTB & ~LED_MASK) | (rgb_led_output & LED_MASK);
            PORTD = (PORTD & ~LED_RGB_MASK) | (rgb_output & LED_RGB_MASK);
        }
        else
        {
            PORTB = (PORTB & ~LED_MASK) | (output & LED_MASK);
        }

        // // check rotor switch status
        // uint8_t current_sw_state = PIND & ROTOR_SW;

        // if (prev_sw_state != current_sw_state)
        // {
        //     prev_sw_debounce = milliSec_get();
        // }

        // if (true_sw_state != current_sw_state)
        // {
        //     if (milliSec_get() - prev_sw_debounce > debounce_time)
        //     {
        //         true_sw_state = current_sw_state;

        //         if (!true_sw_state)
        //         {
        //             sys_mode = RGB;
        //             rotor_sw_select = !rotor_sw_select;
        //             uart_print("rotor switch is pressed.");
        //         } else {
        //             btn_led_reset = 2;
        //         }
        //     }
        // }
        // prev_sw_state = current_sw_state;

        // // check btn_green status
        // uint8_t current_btn_led_state = PINB & BTN_LED;

        // if (prev_btn_led_state != current_btn_led_state)
        // {
        //     prev_btn_led_debounce = milliSec_get();
        // }

        // if (true_btn_led_state != current_btn_led_state)
        // {
        //     if (milliSec_get() - prev_btn_led_debounce > debounce_time)
        //     {
        //         true_btn_led_state = current_btn_led_state;

        //         if (!true_btn_led_state && rotor_sw_select == 1)
        //         {
        //             btn_led_reset = (btn_led_reset + 1) % 2;
        //             uart_print("btn on/off is pressed: ");
        //             uart_print_uint16(btn_led_reset);
        //             uart_print("\r\n");
        //         }
        //     }
        // }
        // prev_btn_led_state = current_btn_led_state;

        // // check btn_red status
        // uint8_t current_btn_reset_state = PINB & BTN_RESET;

        // if (prev_btn_reset_state != current_btn_reset_state)
        // {
        //     prev_btn_reset_debounce = milliSec_get();
        // }

        // if (true_btn_reset_state != current_btn_reset_state)
        // {
        //     if (milliSec_get() - prev_btn_reset_debounce > debounce_time)
        //     {
        //         true_btn_reset_state = current_btn_reset_state;

        //         if (!true_btn_reset_state && rotor_sw_select == 1)
        //         {
        //             btn_led_reset = 2;
        //             uart_print("btn reset is pressed");
        //             uart_print("\r\n");
        //         }
        //     }
        // }
        // prev_btn_reset_state = current_btn_reset_state;

        // if (rotor_sw_select == 1){
        //     updateLEDbits_RGBmode_BtnIsPressed(btn_led_reset);
        // }
    }
}