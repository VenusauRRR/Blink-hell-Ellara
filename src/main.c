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

volatile SYSTEM_MODE sys_mode = DEFAULT;

volatile uint8_t rotor_sw_select = 0;

const unsigned long interval = 250;

volatile uint8_t blink_state = 0;
volatile uint8_t blink_state_rgb = 0;
volatile uint8_t output_default;
volatile uint8_t output_uart;
volatile uint8_t output_led_rgb;
volatile uint8_t output_rgb_rgb;
volatile uint8_t btn_led_reset = _rgb_btn_init;
volatile uint32_t extraTime_BlinkStage;

int main(void)
{
    adc_init(1, 128);
    uart_init(9600);
    timer_init();
    rotor_init();
    sei();

    DDRB |= LED_MASK; // set as output
    PORTB |= LED_MASK;

    DDRD |= LED_RGB_MASK; // set as output
    PORTD |= LED_RGB_MASK;

    DDRB &= ~BTN_LED; // set as input
    PORTB |= BTN_LED; // enable pull up

    DDRB &= ~BTN_RESET; // set as input
    PORTB |= BTN_RESET; // enable pull up

    unsigned long currentMilli = milliSec_get();
    unsigned long previousMilli = 0;

    // Debounce
    unsigned long debounce_time = 20;
    unsigned long prev_sw_debounce = 0;
    unsigned long prev_btn_led_debounce = 0;
    unsigned long prev_btn_reset_debounce = 0;

    uint8_t prev_btn_led_state = PINB & BTN_LED;
    uint8_t true_btn_led_state = prev_btn_led_state;
    uint8_t prev_btn_reset_state = PINB & BTN_RESET;
    uint8_t true_btn_reset_state = prev_btn_reset_state;
    uint8_t prev_sw_state = PIND & ROTOR_SW;
    uint8_t true_sw_state = prev_sw_state;

    extraTime_BlinkStage = 0;

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
            blink_state_rgb ^= LED_RGB_MASK;
            blinkstadiet_count = (blinkstadiet_count + 1) % 4;
        }

        if (blinkstadiet_flag == ON){
            extraTime_BlinkStage = (uint32_t)potentioReading_A1 * 255 / 1023;
        } else{
            extraTime_BlinkStage = 0;
        }

        output_default = blink_state;
        output_uart = blink_state;
        output_led_rgb = blink_state;
        output_rgb_rgb = blink_state_rgb;

        // check rotor switch status
        uint8_t current_sw_state = PIND & ROTOR_SW;

        if (prev_sw_state != current_sw_state)
        {
            sys_mode = RGB;
            prev_sw_debounce = milliSec_get();
        }

        if (true_sw_state != current_sw_state)
        {
            if (milliSec_get() - prev_sw_debounce > debounce_time)
            {
                true_sw_state = current_sw_state;

                if (!true_sw_state)
                {
                    rotor_sw_select = !rotor_sw_select;

                    if (blinkstadiet_flag == ON)
                    {
                        blinkstadiet_flag = OFF;
                    }
                    uart_print("rotor switch is pressed: ");
                    uart_print_uint16(rotor_sw_select);
                    uart_print("\r\n");
                }
                else
                {
                    btn_led_reset = _rgb_btn_init;
                }
            }
        }
        prev_sw_state = current_sw_state;

        // check btn_green status
        uint8_t current_btn_led_state = PINB & BTN_LED;

        if (prev_btn_led_state != current_btn_led_state)
        {
            prev_btn_led_debounce = milliSec_get();
        }

        if (true_btn_led_state != current_btn_led_state)
        {
            if (milliSec_get() - prev_btn_led_debounce > debounce_time)
            {
                true_btn_led_state = current_btn_led_state;

                if (!true_btn_led_state)
                {
                    btn_led_reset = (btn_led_reset + 1) % 2;
                    uart_print("btn on/off is pressed: ");
                    uart_print_uint16(btn_led_reset);
                    uart_print("\r\n");
                }
            }
        }
        prev_btn_led_state = current_btn_led_state;

        // check btn_red status
        uint8_t current_btn_reset_state = PINB & BTN_RESET;

        if (prev_btn_reset_state != current_btn_reset_state)
        {
            prev_btn_reset_debounce = milliSec_get();
        }

        if (true_btn_reset_state != current_btn_reset_state)
        {
            if (milliSec_get() - prev_btn_reset_debounce > debounce_time)
            {
                true_btn_reset_state = current_btn_reset_state;

                if (!true_btn_reset_state)
                {
                    btn_led_reset = _rgb_btn_BLINK;
                    uart_print("btn reset is pressed");
                    uart_print("\r\n");
                }
            }
        }
        prev_btn_reset_state = current_btn_reset_state;

        flag_manager();

        switch (sys_mode)
        {
        case DEFAULT:
            PORTB = ((PORTB & ~LED_MASK) | (output_default & LED_MASK));
            PORTD &= ~LED_RGB_MASK;
            break;
        case RGB:
            PORTB = ((PORTB & ~LED_MASK) | (output_led_rgb & LED_MASK));
            PORTD = ((PORTD & ~LED_RGB_MASK) | (output_rgb_rgb & LED_RGB_MASK));
            break;
        case UART:
            PORTB = ((PORTB & ~LED_MASK) | (output_uart & LED_MASK));
            PORTD &= ~LED_RGB_MASK;
            break;
        default:
            break;
        }
    }
}