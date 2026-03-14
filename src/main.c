#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

#include "../include/millis.h"
#include "../include/adc.h"
#include "../include/uart.h"
#include "../include/definition.h"
#include "../include/rotor.h"
#include "../include/event.h"

volatile SYSTEM_MODE sys_mode = DEFAULT;

Lightings_state lgt_defaultMode;

volatile Lightings_state lgt_uartMode;

volatile Lightings_state lgt_rgbMode;

volatile uint8_t rotor_sw_select = 0;

const unsigned long interval = 250;

int main(void)
{
    adc_init(1, 128);
    uart_init(9600);
    timer_init();
    rotor_init();
    sei();

    DDRB |= (LED_BLUE | LED_GREEN | LED_RED | LED_WHITE);
    DDRD |= (LED_RGB_RED | LED_RGB_GREEN | LED_RGB_BLUE);
    DDRD |= BTN_GREEN;
    DDRD |= BTN_RED;
    DDRD &= ~(ROTOR_SW); // set as input
    PORTD |= ROTOR_SW;

    unsigned long currentMilli = milliSec_get();
    unsigned long previousMilli = 0;

    updateStructLedGroup(&lgt_defaultMode, TOGGLE, TOGGLE, TOGGLE, TOGGLE);
    updateStructRGBGroup(&lgt_defaultMode, OFF, OFF, OFF);

    updateStructLedGroup(&lgt_uartMode, TOGGLE, TOGGLE, TOGGLE, TOGGLE);
    updateStructRGBGroup(&lgt_uartMode, OFF, OFF, OFF);
    
    updateStructLedGroup(&lgt_rgbMode, TOGGLE, TOGGLE, TOGGLE, TOGGLE);
    updateStructRGBGroup(&lgt_rgbMode, OFF, OFF, OFF);

    updateLightingBits(&lgt_defaultMode);

        // Debounce
    unsigned long debounce_time = 20;
    unsigned long prev_sw_debounce = 0;
    uint8_t prev_sw_state = 0; // Knappen hög --> 5V
    uint8_t true_sw_state = 1;

    while (1)
    {
        uint16_t potentioReading_A0 = adc_read(0);
        uint16_t potentioReading_A1 = adc_read(1);
        uint32_t convertedPotentioReading_A0 = (uint32_t)potentioReading_A0 * 255 / 1023;

        currentMilli = milliSec_get();
        if (currentMilli - previousMilli >= interval + milliSec_addTime(convertedPotentioReading_A0))
        {
            previousMilli = currentMilli;

            switch (sys_mode)
            {
            case DEFAULT:
                updateLightingBits(&lgt_defaultMode);
                break;
            case RGB:
                updateLightingBits(&lgt_rgbMode);
                break;
            case UART:
                updateLightingBits(&lgt_uartMode);
                break;
            default:
                break;
            }
        }

         // Togglar lysdioden om knappen är intryckt
        uint8_t current_sw_state = PIND & ROTOR_SW;

        if(prev_sw_state != current_sw_state)
        {
            prev_sw_debounce = milliSec_get();
            // uart_print("hello");
        }
        
        if(true_sw_state != current_sw_state)
        {   
            // uart_print("diff state");
            if (milliSec_get() - prev_sw_debounce > debounce_time)
            {
                // uart_print("debounce > 20");
                true_sw_state = current_sw_state;

                if (!true_sw_state)
                {
                    rotor_sw_select = !rotor_sw_select;
                    uart_print("sw pressed");
                    uart_print_uint16(rotor_sw_select);
                    uart_print("\r\n");
                }
            }
        }
        prev_sw_state = current_sw_state;

        //check rotor switch status
        //if (sw_enable = 1) -> can select color
        //if (sw_enable = 0) -> lock: toggle rgb color

        //check btn_green status
        //if (btn is pressed) -> on/off led color

        //check btn_red status
        //if (btn is pressed) -> reset led color

    }
}