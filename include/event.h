#ifndef EVENT_H
#define EVENT_H

#include <avr/io.h>
#include "../include/definition.h"
#include "../include/bitops.h"


// static const LED_COLOR led_state_list[LED_ST_COUNT] = {
//     LED_ST_RED,
//     LED_ST_GREEN,
//     LED_ST_BLUE,
//     LED_ST_WHITE};

static const ROTOR_COLOR rotor_color_list[ROTOR_COLOR_COUNT] = {
    ROTOR_OFF,
    ROTOR_RED,
    ROTOR_GREEN,
    ROTOR_BLUE,
    ROTOR_WHITE,
    ROTOR_COLOR_COUNT
};

void setBit_LED(LGT_STATE st, LED_COLOR led){
    switch (st)
    {
    case ON:
        // SET_BIT(PORTB, led);
        PORTB |= led;
        break;
    case OFF:
        // CLR_BIT(PORTB, led);
        PORTB &= ~led;
        break;
    case TOGGLE:
        // TOG_BIT(PORTB, led);
        PORTB ^= led;
        break;
    default:
        break;
    }
}

void setBit_RGB(LGT_STATE st, ROTOR_COLOR rgb_x){
    switch (st)
    {
    case ON:
        // SET_BIT(PORTD, rgb_x);
        PORTD |= rgb_x;
        break;
    case OFF:
        // CLR_BIT(PORTD, rgb_x);
        PORTD &= ~rgb_x;
        break;
    case TOGGLE:
        // TOG_BIT(PORTD, rgb_x);
        PORTD ^= rgb_x;
        break;
    default:
        break;
    }
}

//update ON/OFF/TOGGLE in struct LED
void updateStructLedGroup(Lightings_state *st, LGT_STATE red, LGT_STATE green, LGT_STATE blue, LGT_STATE white){
    st->led_red_st = red;
    st->led_green_st = green;
    st->led_blue_st = blue;
    st->led_white_st = white;
}

void updateStructRGBGroup(Lightings_state *st, LGT_STATE red, LGT_STATE green, LGT_STATE blue){
    st->rgb_red_st = red;
    st->rgb_green_st = green;
    st->rgb_blue_st = blue;
}

void updateLightingBits(Lightings_state *st){
    setBit_LED(st->led_red_st, LED_ST_RED);
    setBit_LED(st->led_blue_st, LED_ST_BLUE);
    setBit_LED(st->led_green_st, LED_ST_GREEN);
    setBit_LED(st->led_white_st, LED_ST_WHITE);
    setBit_RGB(st->rgb_red_st, ROTOR_RED);
    setBit_RGB(st->rgb_blue_st, ROTOR_BLUE);
    setBit_RGB(st->rgb_green_st, ROTOR_GREEN);
}

// void updateRGBbits(Lightings_state *st){
// }

// void enable_LED_BlinkStadiet(){

// }

// void select_RGBcolor(){

// }

// void runSysMode_Default(){
// }

// void updateSystemMode(SYSTEM_MODE mode){
//     switch (mode)
//     {
//     case DEFAULT:
//         /* code */
//         break;
//     case RGB:
//         /* code */
//         break;
//     case UART:
//         /* code */
//         break;
//     default:
//         break;
//     }
// }

#endif