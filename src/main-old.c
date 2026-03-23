// #include <avr/io.h>
// #include <util/delay.h>
// #include <stdio.h>
// #include <string.h>

// #include "../include/millis.h"
// #include "../include/adc.h"
// #include "../include/uart.h"
// #include "../include/definition.h"
// #include "../include/rotor.h"
// #include "../include/event.h"

// volatile SYSTEM_MODE sys_mode = DEFAULT;

// Lightings_state lgt_defaultMode;

// volatile Lightings_state lgt_uartMode;

// volatile Lightings_state lgt_rgbMode;

// volatile uint8_t rotor_sw_select = 0;
// volatile uint8_t btn_led_reset = 2;

// const unsigned long interval = 250;

// int main(void)
// {
//     adc_init(1, 128);
//     uart_init(9600);
//     timer_init();
//     rotor_init();
//     sei();

//     DDRB |= (LED_BLUE | LED_GREEN | LED_RED | LED_WHITE);
//     DDRD |= (LED_RGB_RED | LED_RGB_GREEN | LED_RGB_BLUE);
//     DDRD |= BTN_LED;
//     DDRD |= BTN_RESET;
//     DDRD &= ~(ROTOR_SW); // set as input
//     PORTD |= ROTOR_SW;

//     unsigned long currentMilli = milliSec_get();
//     unsigned long previousMilli = 0;

//     updateStructLedGroup(&lgt_defaultMode, TOGGLE, TOGGLE, TOGGLE, TOGGLE);
//     updateStructRGBGroup(&lgt_defaultMode, OFF, OFF, OFF);

//     updateStructLedGroup(&lgt_uartMode, TOGGLE, TOGGLE, TOGGLE, TOGGLE);
//     updateStructRGBGroup(&lgt_uartMode, OFF, OFF, OFF);

//     updateStructLedGroup(&lgt_rgbMode, TOGGLE, TOGGLE, TOGGLE, TOGGLE);
//     updateStructRGBGroup(&lgt_rgbMode, OFF, OFF, OFF);

//     updateLightingBits(&lgt_defaultMode);

//     // Debounce
//     unsigned long debounce_time = 20;
//     unsigned long prev_sw_debounce = 0;
//     unsigned long prev_btn_led_debounce = 0;
//     unsigned long prev_btn_reset_debounce = 0;
//     uint8_t prev_sw_state = 0; // Knappen hög --> 5V
//     uint8_t true_sw_state = 1;
//     uint8_t prev_btn_led_state = 0; // Knappen hög --> 5V
//     uint8_t true_btn_led_state = 1;
//     uint8_t prev_btn_reset_state = 0; // Knappen hög --> 5V
//     uint8_t true_btn_reset_state = 1;

//     uint32_t extraTime_BlinkStage = 0;

//     while (1)
//     {
//         uint16_t potentioReading_A0 = adc_read(0);
//         uint16_t potentioReading_A1 = adc_read(1);
//         uint32_t convertedPotentioReading_A0 = (uint32_t)potentioReading_A0 * 255 / 1023;

//         currentMilli = milliSec_get();
//         if (currentMilli - previousMilli >= interval + milliSec_addTime(convertedPotentioReading_A0) + milliSec_addTime(extraTime_BlinkStage))
//         {
//             previousMilli = currentMilli;

//             switch (sys_mode)
//             {
//             case DEFAULT:
//                 updateLightingBits(&lgt_defaultMode);
//                 break;
//             case RGB:
//                 updateLightingBits(&lgt_rgbMode);
//                 break;
//             case UART:
//                 updateLightingBits(&lgt_uartMode);
//                 break;
//             default:
//                 break;
//             }

//             if (rotor_sw_select)
//             {
//                 updateRGBcolor_switchIsPressed();
//                 if (led_blinkstadiet == 1)
//                 {
//                     extraTime_BlinkStage = (uint32_t)potentioReading_A1 * 255 / 1023;
//                     updateLEDbits_blinkstadiet();
//                     updateLightingBits(&lgt_rgbMode);
//                     led_state_idx = (led_state_idx + 1) % 4;
//                 }
//             }
//             else
//             {
//                 led_blinkstadiet = 0;
//                 extraTime_BlinkStage = 0;
//                 updateStructLedGroup(&lgt_rgbMode, OFF, OFF, OFF, OFF);
//                 updateStructLedGroup(&lgt_rgbMode, TOGGLE, TOGGLE, TOGGLE, TOGGLE);
//                 updateRGBcolor_switchIsNotPressed();
//             }
//         }

//         //check rotor switch status
//         uint8_t current_sw_state = PIND & ROTOR_SW;

//         if (prev_sw_state != current_sw_state)
//         {
//             sys_mode = RGB;
//             prev_sw_debounce = milliSec_get();
//         }

//         if (true_sw_state != current_sw_state)
//         {
//             if (milliSec_get() - prev_sw_debounce > debounce_time)
//             {
//                 true_sw_state = current_sw_state;

//                 if (!true_sw_state)
//                 {
//                     rotor_sw_select = !rotor_sw_select;
//                     uart_print("rotor switch is pressed.");
//                 }
//             }
//         }
//         prev_sw_state = current_sw_state;

//         // check btn_green status
//         uint8_t current_btn_led_state = PINB & BTN_LED;

//         if (prev_btn_led_state != current_btn_led_state)
//         {
//             prev_btn_led_debounce = milliSec_get();
//         }

//         if (true_btn_led_state != current_btn_led_state)
//         {
//             if (milliSec_get() - prev_btn_led_debounce > debounce_time)
//             {
//                 true_btn_led_state = current_btn_led_state;

//                 if (!true_btn_led_state && rotor_sw_select == 1)
//                 {
//                     btn_led_reset = (btn_led_reset + 1) % 2;
//                     uart_print("btn on/off is pressed: ");
//                     uart_print_uint16(btn_led_reset);
//                     uart_print("\r\n");
//                 }
//             }
//         }
//         prev_btn_led_state = current_btn_led_state;

//         // check btn_red status
//         uint8_t current_btn_reset_state = PINB & BTN_RESET;

//         if (prev_btn_reset_state != current_btn_reset_state)
//         {
//             prev_btn_reset_debounce = milliSec_get();
//         }

//         if (true_btn_reset_state != current_btn_reset_state)
//         {
//             if (milliSec_get() - prev_btn_reset_debounce > debounce_time)
//             {
//                 true_btn_reset_state = current_btn_reset_state;

//                 if (!true_btn_reset_state && rotor_sw_select == 1)
//                 {
//                     btn_led_reset = 2;
//                     uart_print("btn reset is pressed");
//                     uart_print("\r\n");
//                 }
//             }
//         }
//         prev_btn_reset_state = current_btn_reset_state;

//         updateLEDbits_RGBmode_BtnIsPressed(btn_led_reset);
//     }
// }