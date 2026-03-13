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

LedGroup_state leds_defaultMode;
RgbGroup_state rgb_defaultMode;

// volatile leds_rgbMode;
// volatile rgb_rgbMode;

volatile LedGroup_state leds_uartMode;
volatile RgbGroup_state rgb_uartMode;

const unsigned long interval = 250;

int main(void)
{
    adc_init(1, 128);
    uart_init(9600);
    timer_init();
    // rotor_init();
    sei();

    DDRB |= (LED_BLUE | LED_GREEN | LED_RED | LED_WHITE);
    DDRD |= (LED_RGB_RED | LED_RGB_GREEN | LED_RGB_BLUE);

    unsigned long currentMilli = milliSec_get();
    unsigned long previousMilli = 0;

    updateStructLedGroup(&leds_defaultMode, TOGGLE, TOGGLE, TOGGLE, TOGGLE);
    updateStructRGBGroup(&rgb_defaultMode, OFF, OFF, OFF);
    updateStructLedGroup(&leds_uartMode, TOGGLE, TOGGLE, TOGGLE, TOGGLE);
    updateStructRGBGroup(&rgb_uartMode, OFF, OFF, OFF);

    updateLEDsBits(&leds_defaultMode);
    updateRGBbits(&rgb_defaultMode);

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
                updateLEDsBits(&leds_defaultMode);
                break;
            case UART:
                updateLEDsBits(&leds_uartMode);
                break;
            default:
                break;
            }
        }
    }
}