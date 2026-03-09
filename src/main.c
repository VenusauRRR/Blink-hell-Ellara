#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "../include/millis.h"
#include "../include/adc.h"
#include "../include/uart.h"
#include "../include/definition.h"
#include "../include/button.h"

const unsigned long interval = 250;

static const LED_STATE led_state_list[LED_ST_COUNT] = {
    LED_ST_RED,
    LED_ST_GREEN,
    LED_ST_BLUE,
    LED_ST_WHITE};

int main(void)
{
    timer_init();
    rotor_init();
    // button_init();
    sei();

    adc_init(1, 128);
    uart_init(9600);

    DDRB |= LED_BLUE;
    DDRB |= LED_GREEN;
    DDRB |= LED_RED;
    DDRB |= LED_WHITE;

    PORTB |= LED_BLUE;
    PORTB |= LED_GREEN;
    PORTB |= LED_RED;
    PORTB |= LED_WHITE;

    DDRD |= LED_RGB_RED;
    DDRD |= LED_RGB_GREEN;
    DDRD |= LED_RGB_BLUE;

    DDRD &= ~(ROTOR_SW); // set as input
    PORTD |= ROTOR_SW;

    unsigned long currentMilli = milliSec_get();
    unsigned long previousMilli = 0;
    unsigned long currentMilli_sw = milliSec_get();
    unsigned long previousMilli_sw = 0;
    unsigned long currentMilli_rgb = milliSec_get();
    unsigned long previousMilli_rgb = 0;

    unsigned long currentMilli_btn = milliSec_get();
    unsigned long previousMilli_btn_red = 0;
    unsigned long previousMilli_btn_green = 0;

    uint8_t last_rotor_sw_state = 1;
    uint8_t last_btn_green_state = 1;
    uint8_t rotor_sw_enable = 0;
    uint8_t rgb_color_state = 0;
    uint8_t led_toggle = 2;

    uint8_t blink_mask = (LED_BLUE | LED_GREEN | LED_RED | LED_WHITE);

    while (1)
    {
        uint16_t potentioReading = adc_read(0);
        uint32_t convertedPotentioReading = (uint32_t)potentioReading * 255 / 1023;

        currentMilli = milliSec_get();
        if (currentMilli - previousMilli >= interval + milliSec_addTime(convertedPotentioReading))
        {
            previousMilli = currentMilli;

            
            PORTB ^= blink_mask;
            uint8_t led = led_state_list[rotor_state_idx-1];
            
            
            if (led_toggle == 0){
                blink_mask &= ~led;
                PORTB &= ~led;
            } else if (led_toggle == 1){
                blink_mask &= ~led;
                PORTB &= ~led;
                PORTB |= led;
            } else if (led_toggle == 2){
                blink_mask |= led;
            }
        }
        
        if (rotor_sw_enable == 1)
        {
            // Disable INT1
            EIMSK &= ~(1 << INT0);
            currentMilli_rgb = milliSec_get();
            if (currentMilli_rgb - previousMilli_rgb >= 100)
            {
                previousMilli_rgb = currentMilli_rgb;
                PORTD ^= rgb_color_state;
            }
        }
        else
        {
            EIMSK |= (1 << INT0);
            PORTD &= ~(LED_RGB_MASK);
            PORTD |= rotor_state_list[rotor_state_idx];
        }

        uint8_t rotor_sw_state = ((PIND & ROTOR_SW) ? 1 : 0);
        currentMilli_sw = milliSec_get();
        if (last_rotor_sw_state == 1 && rotor_sw_state == 0)
        {
            if (currentMilli_sw - previousMilli_sw >= 2000)
            {
                previousMilli_sw = currentMilli_sw;
                uart_print("sw start");
                rotor_sw_enable = !rotor_sw_enable;
                rgb_color_state = PIND & LED_RGB_MASK;
            }
        }

        uint8_t btn_green_state = ((PINB & BTN_GREEN) ? 1 : 0);
        uint8_t btn_red_state = ((PINB & BTN_RED) ? 1 : 0);
        if (last_btn_green_state == 1 && btn_green_state == 0)
        {
            currentMilli_btn = milliSec_get();
            if (currentMilli_btn - previousMilli_btn_green >= 1000)
            {
                previousMilli_btn_green = currentMilli_btn;
                uart_print("Green");
                led_toggle = (led_toggle + 1) % 2;
            }
        }
        if (last_btn_red_state == 1 && btn_red_state == 0)
        {
            currentMilli_btn = milliSec_get();
            if (currentMilli_btn - previousMilli_btn_red >= 1000)
            {
                previousMilli_btn_red = currentMilli_btn;
                uart_print("Red");
                led_toggle = 2;
            }
        }
        last_btn_green_state = btn_green_state;
        last_btn_red_state = btn_red_state;

        // if (btn_red_flag == 1){
        //     uart_print("red");
        //     btn_red_flag = 0;
        // }
        // if (btn_green_flag == 1){

        //     uart_print("green");
        //     btn_green_flag = 0;
        // }
    }
}