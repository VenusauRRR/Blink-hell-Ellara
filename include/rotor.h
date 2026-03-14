#ifndef ROTOR_H
#define ROTOR_H

#include "../include/definition.h"
#include "../include/millis.h"
#include "../include/uart.h"

// static volatile uint8_t last_rotor_clk_state = 1;
volatile uint8_t rotor_state_idx = 0;
volatile uint8_t led_blinkstadiet = 0;
// unsigned long currentMilli_btn = 0;
// unsigned long previousMilli_btn_red = 0;
// unsigned long previousMilli_btn_green = 0;
// volatile uint8_t last_btn_red_state = 0;
// volatile uint8_t last_btn_green_state = 0;

// volatile uint8_t btn_red_flag = 0;
// volatile uint8_t btn_green_flag = 0;

void updateRGBcolor_switchIsPressed(){
    led_blinkstadiet = 0;
    ROTOR_COLOR c = rotor_color_list[rotor_state_idx];
    switch (c)
    {
    case ROTOR_OFF:
        led_blinkstadiet = 1;
        break;
    case ROTOR_RED:
        lgt_rgbMode.rgb_red_st = TOGGLE;
        lgt_rgbMode.rgb_green_st = lgt_rgbMode.rgb_blue_st = OFF;
        break;
    case ROTOR_GREEN:
        lgt_rgbMode.rgb_green_st = TOGGLE;
        lgt_rgbMode.rgb_red_st = lgt_rgbMode.rgb_blue_st = OFF;
        break;
    case ROTOR_BLUE:
        lgt_rgbMode.rgb_blue_st = TOGGLE;
        lgt_rgbMode.rgb_red_st = lgt_rgbMode.rgb_green_st = OFF;
        break;
    case ROTOR_WHITE:
        lgt_rgbMode.rgb_red_st = lgt_rgbMode.rgb_green_st = lgt_rgbMode.rgb_blue_st = TOGGLE;
        break;
    default:
        break;
    }
}

void updateRGBcolor_switchIsNotPressed()
{
    ROTOR_COLOR c = rotor_color_list[rotor_state_idx];

    switch (c)
    {
    case ROTOR_OFF:
        lgt_rgbMode.rgb_red_st = lgt_rgbMode.rgb_green_st = lgt_rgbMode.rgb_blue_st = OFF;
        break;
    case ROTOR_RED:
        lgt_rgbMode.rgb_red_st = ON;
        lgt_rgbMode.rgb_green_st = lgt_rgbMode.rgb_blue_st = OFF;
        break;
    case ROTOR_GREEN:
        lgt_rgbMode.rgb_green_st = ON;
        lgt_rgbMode.rgb_red_st = lgt_rgbMode.rgb_blue_st = OFF;
        break;
    case ROTOR_BLUE:
        lgt_rgbMode.rgb_blue_st = ON;
        lgt_rgbMode.rgb_red_st = lgt_rgbMode.rgb_green_st = OFF;
        break;
    case ROTOR_WHITE:
        lgt_rgbMode.rgb_red_st = lgt_rgbMode.rgb_green_st = lgt_rgbMode.rgb_blue_st = ON;
        break;
    default:
        break;
    }
}

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
    sys_mode = RGB;
    led_blinkstadiet = 0;

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
        updateRGBcolor_switchIsNotPressed();
    }
}

#endif