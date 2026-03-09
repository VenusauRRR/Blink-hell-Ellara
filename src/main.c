#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "../include/millis.h"
#include "../include/adc.h"
#include "../include/uart.h"
#include "../include/definition.h"
#include "../include/button.h"
#include "../include/button.h"

const unsigned long interval = 250;

int main(void)
{
    timer_init();
    rotor_init();
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
    uint8_t last_rotor_sw_state = 1;
    uint8_t rotor_sw_enable = 0;
    uint8_t rgb_color_state = 0;

    while (1)
    {
        uint16_t potentioReading = adc_read(0);
        uint32_t convertedPotentioReading = (uint32_t)potentioReading * 255 / 1023;

        currentMilli = milliSec_get();
        if (currentMilli - previousMilli >= interval + milliSec_addTime(convertedPotentioReading))
        {
            previousMilli = currentMilli;

            PORTB ^= LED_BLUE;
            PORTB ^= LED_GREEN;
            PORTB ^= LED_RED;
            PORTB ^= LED_WHITE;
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
    }
}