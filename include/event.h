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

static const RGB_COLOR rotor_color_list[RGB_COUNT] = {
    RGB_OFF,
    RGB_RED,
    RGB_GREEN,
    RGB_BLUE,
    RGB_WHITE};

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

void setBit_RGB(LGT_STATE st, RGB_COLOR rgb_x){
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
void updateStructLedGroup(LedGroup_state *st, LGT_STATE red, LGT_STATE green, LGT_STATE blue, LGT_STATE white){
    st->red_st = red;
    st->green_st = green;
    st->blue_st = blue;
    st->white_st = white;
}

void updateStructRGBGroup(RgbGroup_state *st, LGT_STATE red, LGT_STATE green, LGT_STATE blue){
    st->red_st = red;
    st->green_st = green;
    st->blue_st = blue;
}

void updateLEDsBits(LedGroup_state *st){
    setBit_LED(st->red_st, LED_ST_RED);
    setBit_LED(st->blue_st, LED_ST_BLUE);
    setBit_LED(st->green_st, LED_ST_GREEN);
    setBit_LED(st->white_st, LED_ST_WHITE);
}

void updateRGBbits(RgbGroup_state *st){
    setBit_RGB(st->red_st, RGB_RED);
    setBit_RGB(st->blue_st, RGB_BLUE);
    setBit_RGB(st->green_st, RGB_GREEN);
}

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