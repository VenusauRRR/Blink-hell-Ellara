#ifndef BUTTON_H
#define BUTTON_H

#include "../include/definition.h"

static volatile uint8_t last_rotor_clk_state = 1;
static volatile uint8_t rotor_state_idx = 0;
static volatile 


static const ROTOR_STATE rotor_state_list[ROTOR_ST_COUNT] = {
    ROTOR_ST_OFF,
    ROTOR_ST_RED,
    ROTOR_ST_GREEN,
    ROTOR_ST_BLUE,
    ROTOR_ST_ALL};

static void rotor_init(void)
{
    DDRD &= ~(ROTOR_CLK | ROTOR_DT | ROTOR_SW); // set as input
    PORTD |= ROTOR_CLK | ROTOR_DT | ROTOR_SW;

    EICRA |= (1 << ISC01); // falling edge CLK DT
    EICRA &= ~(1 << ISC00);

    EICRA |= (1 << ISC11);// falling edge SW
EICRA &= ~(1 << ISC10); 

    EIMSK |= (1 << INT0); // enable INT0
    EIMSK |= (1 << INT1);// enable INT1
}

ISR(INT0_vect)
{
    uint8_t rotor_dt_state = ((PIND & ROTOR_DT) ? 1 : 0);

    if (rotor_dt_state != 0)
    {
        rotor_state_idx = (rotor_state_idx + 1) % ROTOR_ST_COUNT;
    }
    else
    {
        rotor_state_idx = (rotor_state_idx == 0 ? (ROTOR_ST_COUNT - 1) : (rotor_state_idx - 1));
    }
    PORTD &= ~LED_RGB_MASK;
    PORTD |= rotor_state_list[rotor_state_idx];
}


ISR(INT1_vect){

}
#endif