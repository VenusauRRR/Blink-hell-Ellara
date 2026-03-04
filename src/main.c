#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "../include/myTimer.h"
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

    // DDRD &= ~ROTOR_CLK; // set as input
    // DDRD &= ~ROTOR_DT;  // set as input

    // PORTD |= ROTOR_CLK | ROTOR_DT;

    unsigned long currentMilli = milliSec_get();
    unsigned long previousMilli = 0;

    // uint8_t last_rotor_clk_state = 1;
    // uint8_t rotor_state_idx = 0;

    // ROTOR_STATE rotor_state_list[ROTOR_ST_COUNT] = {
    //     ROTOR_ST_OFF,
    //     ROTOR_ST_RED,
    //     ROTOR_ST_GREEN,
    //     ROTOR_ST_BLUE,
    //     ROTOR_ST_ALL
    // };

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
            // PORTB ^= LED_RGB_RED;
            // PORTD ^= LED_RGB_GREEN;
            // PORTD ^= LED_RGB_BLUE;
        }

        // // uint8_t rotor_clk_state = (PIND & ROTOR_CLK);
        // uint8_t rotor_clk_state = ((PIND & ROTOR_CLK) ? 1 : 0);

        // if (rotor_clk_state != last_rotor_clk_state)
        // {
        //     last_rotor_clk_state = rotor_clk_state;
        //     uint8_t rotor_dt_state = ((PIND & ROTOR_DT) ? 1 : 0);
        //     // uint8_t rotor_dt_state = (PIND & ROTOR_DT);

        //     if (rotor_clk_state == 0 && rotor_dt_state != 0)
        //     {
        //         rotor_state_idx = (rotor_state_idx + 1) % ROTOR_ST_COUNT;
        //         PORTD &= ~LED_RGB_MASK;
        //         PORTD |= rotor_state_list[rotor_state_idx];
        //     }
        //     if (rotor_clk_state == 0 && rotor_dt_state == 0)
        //     {
        //         rotor_state_idx = (rotor_state_idx == 0 ? (ROTOR_ST_COUNT - 1) : (rotor_state_idx - 1));
        //         PORTD &= ~LED_RGB_MASK;
        //         PORTD |= rotor_state_list[rotor_state_idx];
        //     }
        // }
    }
}