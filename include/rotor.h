#ifndef BUTTON_H
#define BUTTON_H

#include "../include/definition.h"
#include "../include/millis.h"
#include "../include/uart.h"

static volatile uint8_t last_rotor_clk_state = 1;
uint8_t rotor_state_idx = 0;
unsigned long currentMilli_btn = 0;
unsigned long previousMilli_btn_red = 0;
unsigned long previousMilli_btn_green = 0;
volatile uint8_t last_btn_red_state = 0;
volatile uint8_t last_btn_green_state = 0;

volatile uint8_t btn_red_flag = 0;
volatile uint8_t btn_green_flag = 0;

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

    // falling edge for CLK and DT
    EICRA |= (1 << ISC01); 
    EICRA &= ~(1 << ISC00);

    EIMSK |= (1 << INT0); // enable INT0
}

ISR(INT0_vect)
{
    led_stir_mode = 0;
    uint8_t clk = (PIND & ROTOR_CLK) ? 1 : 0;
    uint8_t rotor_dt_state = ((PIND & ROTOR_DT) ? 1 : 0);

    if (clk == 0 && rotor_dt_state != 0)
    {
        rotor_state_idx = (rotor_state_idx + 1) % ROTOR_ST_COUNT;
    }
    else if (clk == 0 && rotor_dt_state == 0)
    {
        rotor_state_idx = (rotor_state_idx == 0 ? (ROTOR_ST_COUNT - 1) : (rotor_state_idx - 1));
    }
    PORTD &= ~LED_RGB_MASK;
    PORTD |= rotor_state_list[rotor_state_idx];
}

#endif