#ifndef MYTIMER_H
#define MYTIMER_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdbool.h>

volatile unsigned long milisec = 0;

void timer_init(void){
    //Timer0 is used for counting milli sec

    //set CTC mode
    TCCR0A |= (1 << WGM01);
    
    //set prescaler
    TCCR0B |= (1 << CS01) | (1 << CS00); 

    //set compare value for 1 milli second
    OCR0A = 249;

    // set time interrupt mask
    TIMSK0 |= (1 << OCIE0A);
}

//time interrupt for Timer0: milli second
ISR(TIMER0_COMPA_vect) {
    milisec++;
}

unsigned long getMilliSec(){
    return milisec;
}

#endif