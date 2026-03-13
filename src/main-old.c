// #include <avr/io.h>
// #include <util/delay.h>
// #include <stdio.h>
// #include <string.h>

// #include "../include/millis.h"
// #include "../include/adc.h"
// #include "../include/uart.h"
// #include "../include/definition.h"
// #include "../include/rotor.h"

// const unsigned long interval = 250;

// uint8_t rgb_led_toggle = 0;
// uint8_t led = 0;
// uint8_t led_mask = (LED_BLUE | LED_GREEN | LED_RED | LED_WHITE);

// static const LED_STATE led_state_list[LED_ST_COUNT] = {
//     LED_ST_RED,
//     LED_ST_GREEN,
//     LED_ST_BLUE,
//     LED_ST_WHITE};

// int main(void)
// {
//     adc_init(1, 128);
//     uart_init(9600);
//     timer_init();
//     rotor_init();
//     sei();

//     DDRB |= led_mask;
//     PORTB |= led_mask;

//     DDRD |= LED_RGB_RED;
//     DDRD |= LED_RGB_GREEN;
//     DDRD |= LED_RGB_BLUE;

//     DDRD &= ~(ROTOR_SW); // set as input
//     PORTD |= ROTOR_SW;

//     unsigned long currentMilli = milliSec_get();
//     unsigned long previousMilli = 0;
//     unsigned long currentMilli_sw = milliSec_get();
//     unsigned long previousMilli_sw = 0;
//     unsigned long currentMilli_rgb = milliSec_get();
//     unsigned long previousMilli_rgb = 0;

//     unsigned long currentMilli_btn = milliSec_get();
//     unsigned long previousMilli_btn_red = 0;
//     unsigned long previousMilli_btn_green = 0;

//     uint8_t last_rotor_sw_state = 1;
//     uint8_t last_btn_green_state = 1;
//     uint8_t rotor_sw_enable = 0;
//     uint8_t rgb_color_state = 0;
//     uint8_t led_off_idx = 0;
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

//             PORTB ^= led_mask;
//             uint8_t led = led_state_list[rotor_state_idx - 1];
            
//             if (led_stir_mode == 1){
//                 rotor_sw_enable = 0;
//                 led_mask |= led;
//                 PORTD |= ROTOR_ST_OFF;
//                 switch (led_stir_choice)
//                 {
//                 case DISABLE:{
//                     led_mask &= ~led_stir_color;
//                     PORTB ^= led_mask;
//                     break;
//                 }
//                     case ENABLE:{
//                         PORTB |= led_stir_color;
//                         break;
//                     }
//                     case TOGGLE:{
//                         led_mask |= led_stir_color;
//                         break;
//                     }
//                 default:
//                     break;
//                 }
//             }

//             if (rgb_led_toggle == 0)
//             {
//                 if (rotor_state_idx == 0)
//                 {
//                     PORTB &= ~led_mask;
//                     rgb_led_toggle = 2;
//                 }
//                 else
//                 {
//                     led_mask &= ~led;
//                     PORTB &= ~led;
//                 }
//             }
//             //set certain LED to ON
//             else if (rgb_led_toggle == 1)
//             {
//                 if (rotor_state_idx == 0)
//                 {
//                     extraTime_BlinkStage = (uint32_t)potentioReading_A1 * 255 / 1023;
//                     PORTB &= ~led_mask;
//                     led_mask &= ~led_state_list[led_off_idx];
//                     PORTB |= led_mask;
//                     led_mask |= led_state_list[led_off_idx];
//                     led_off_idx = (led_off_idx + 1) % 4;
//                 }
//                 else
//                 {
//                     extraTime_BlinkStage = 0;
//                     led_mask &= ~led;
//                     PORTB &= ~led;
//                     PORTB |= led;
//                 }
//             }
//             // reset LED to blinking mode
//             if (rgb_led_toggle == 2)
//             {
//                 led_mask |= led;
//             }
//         }

//         if (rotor_sw_enable == 1)
//         {
//             // Disable INT1
//             EIMSK &= ~(1 << INT0);
//             currentMilli_rgb = milliSec_get();
//             if (currentMilli_rgb - previousMilli_rgb >= 100)
//             {
//                 previousMilli_rgb = currentMilli_rgb;
//                 PORTD ^= rgb_color_state;
//             }
//         }
//         else
//         {
//             EIMSK |= (1 << INT0);
//             PORTD &= ~(LED_RGB_MASK);
//             PORTD |= rotor_state_list[rotor_state_idx];
//         }

//         uint8_t rotor_sw_state = ((PIND & ROTOR_SW) ? 1 : 0);
//         currentMilli_sw = milliSec_get();
//         if (last_rotor_sw_state == 1 && rotor_sw_state == 0)
//         {
//             if (currentMilli_sw - previousMilli_sw >= 2000)
//             {
//                 previousMilli_sw = currentMilli_sw;
//                 rotor_sw_enable = !rotor_sw_enable;
//                 rgb_color_state = PIND & LED_RGB_MASK;
//             }
//         }

//         uint8_t btn_green_state = ((PINB & BTN_GREEN) ? 1 : 0);
//         uint8_t btn_red_state = ((PINB & BTN_RED) ? 1 : 0);
//         if (last_btn_green_state == 1 && btn_green_state == 0)
//         {
//             currentMilli_btn = milliSec_get();
//             if (currentMilli_btn - previousMilli_btn_green >= 1000)
//             {
//                 previousMilli_btn_green = currentMilli_btn;
//                 rgb_led_toggle = (rgb_led_toggle + 1) % 2;
//             }
//         }
//         if (last_btn_red_state == 1 && btn_red_state == 0)
//         {
//             currentMilli_btn = milliSec_get();
//             if (currentMilli_btn - previousMilli_btn_red >= 1000)
//             {
//                 previousMilli_btn_red = currentMilli_btn;
//                 rgb_led_toggle = 2;
//             }
//         }
//         last_btn_green_state = btn_green_state;
//         last_btn_red_state = btn_red_state;

//     }
// }