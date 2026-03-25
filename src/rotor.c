
#include "../include/rotor.h"

#include <avr/io.h>
#include "../include/definition.h"
#include <avr/interrupt.h>

volatile uint8_t rotor_state_idx = 0;
volatile uint8_t led_blinkstadiet = 0;

void rotor_init(void)
{
    DDRD &= ~(ROTOR_CLK | ROTOR_DT | ROTOR_SW); // set as input
    PORTD |= ROTOR_CLK | ROTOR_DT | ROTOR_SW;

    // falling edge for CLK and DT
    EICRA |= (1 << ISC01);
    EICRA &= ~(1 << ISC00);

    EIMSK |= (1 << INT0); // enable INT0
}

ISR(INT0_vect)
{
    sys_mode = RGB;

    if (rotor_sw_select == 0)
    {
        uint8_t clk = (PIND & ROTOR_CLK) ? 1 : 0;
        uint8_t rotor_dt_state = ((PIND & ROTOR_DT) ? 1 : 0);

        if (clk == 0 && rotor_dt_state != 0)
        {
            rotor_state_idx = (rotor_state_idx + 1) % ROTOR_COLOR_COUNT;
        }
        else if (clk == 0 && rotor_dt_state == 0)
        {
            rotor_state_idx = (rotor_state_idx == 0 ? (ROTOR_COLOR_COUNT - 1) : (rotor_state_idx - 1));
        }
    }
}