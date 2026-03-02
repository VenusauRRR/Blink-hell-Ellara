#include <avr/io.h>
#include <util/delay.h>

#include "../include/myTimer.h"

#define LED_WHITE (1<<PB2)
#define LED_RED (1<<PB5)
#define LED_GREEN (1<<PB4)
#define LED_BLUE (1<<PB3)

const unsigned long interval = 1000;

int main(void){
    timer_init();
    sei();

    DDRB |= LED_BLUE;
    DDRB |= LED_GREEN;
    DDRB |= LED_RED;
    DDRB |= LED_WHITE;

    unsigned long currentMilli = getMilliSec();
    unsigned long previousMilli = 0;
    PORTB |= LED_BLUE;
    PORTB |= LED_GREEN;
    PORTB |= LED_RED;
    PORTB |= LED_WHITE;
    while (1)
    {
        currentMilli = getMilliSec();
        if (currentMilli - previousMilli >= interval){
            previousMilli = currentMilli;
    
            PORTB ^= LED_BLUE;
            PORTB ^= LED_GREEN;
            PORTB ^= LED_RED;
            PORTB ^= LED_WHITE;
           
        }
    }
    



    // // Yello led
    // DDRD |= (1 << PD4);
    // // Blue led
    // DDRD |= (1 << PD6);
    // // Green led
    // DDRB |= (1 << PB0);
    // // Red led
    // DDRB |= (1 << PB3);


    // PORTB |= (1 << PD0);
    // int toggle1 = 0;
    // int toggle2 = 0;

    // while (1)
    // {
    //     PORTB ^= (1 << PB3);
    //     PORTB ^= (1 << PB0);

    //     _delay_ms(250);

    //     if (toggle2)
    //     {
    //         PORTD ^= (1 << PD6);
    //         if (toggle1){
    //             PORTB ^= (1 << PB0);                
    //         }
    //     }
    //     if (toggle1)
    //     {
    //         PORTD ^= (1 << PD4);
    //         toggle2 = !toggle2;
    //     }

    //     _delay_ms(250);


    //     toggle1 = !toggle1;
        
    
    //     // PORTD |= (1 << PD5);
    //     // _delay_ms(250);
    //     // PORTD &= ~(1 << PD5);
    //     // _delay_ms(250);

    // }
    
}

