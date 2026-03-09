#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "../include/millis.h"
#include "../include/adc.h"
#include "../include/uart.h"
#include "../include/definition.h"
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

    unsigned long currentMilli = milliSec_get();
    unsigned long previousMilli = 0;
     unsigned long previousMilli_rgb = 0;

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

        // if (rotor_enable == 1)
        // {
        //     currentMilli = milliSec_get();
        //     if (currentMilli - previousMilli_rgb >= 1000)
        //     {
        //         previousMilli_rgb = currentMilli;
        //         // PORTD &= ~LED_RGB_MASK;
        //         PORTD ^= chosen_rgb_state;
        //     }
        // }
    }
}