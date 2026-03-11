#include "../include/uart.h"
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../include/definition.h"
#include "string.h"

#define F_CPU 16000000UL

static volatile char input[20];
static volatile uint8_t i = 0;
static volatile uint8_t uart_message_ready = 0;


void stirLEDfromUART(const char* input){
    if (strcmp(input, "off") == 0){
        rotor_state_idx = 1;
        led_toggle = 0;
        // led = LED_BLUE;
        // led_mask &= ~LED_BLUE;
    } else if (strcmp(input, "on") == 0){
        // led_mask |= LED_BLUE;
        
        rotor_state_idx = 1;
        led_toggle = 1;
        // led = LED_BLUE;
    }
    else if (strcmp(input, "toggle") == 0){
        // led_mask |= LED_BLUE;
        // PORTB ^= led_mask;
        
        rotor_state_idx = 1;
        led_toggle = 2;
        // led = LED_BLUE;
    }
}

void uart_init(unsigned long baud)
{
    unsigned int ubrr = (F_CPU / 16 / baud) - 1;

    // Sätt baud rate
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;

    // Aktivera sändning och mottagning
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);

    // Sätt ramformat: 8 databitar, 1 stoppbit
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

    // led_toggle_ptr = *led_toggle;
    // rotor_state_idx_ptr = *rotor_state_idx;
}

void uart_transmit(char data)
{
    // Vänta tills sändbufferten är tom
    while (!(UCSR0A & (1 << UDRE0)))
        ;

    // Lägg data i bufferten
    UDR0 = data;
}

char uart_receive(void)
{
    // Vänta tills data har mottagits
    while (!(UCSR0A & (1 << RXC0)))
        ;

    return UDR0;
}

void uart_print(const char *str)
{
    while (*str)
    {
        uart_transmit(*str++);
    }
}

void uart_print_uint16(uint16_t value)
{
    char buffer[6]; // max 65535 + null
    itoa(value, buffer, 10);
    uart_print(buffer);
}

ISR(USART_RX_vect)
{
    // uart_print("-");
    char a = UDR0;
    if (a != '\n' && a != '\r')
    {
        // uart_transmit(a);
        if (i < 19)
        {
            input[i] = a;
            i++;
        }
        else
        {
            input[20] = '\0';
            i = 0;
            // uart_transmit('\n');

            // uart_transmit('A');
            uart_print((char *)input);
            stirLEDfromUART(input);
        }
    }
    else
    {
        input[i] = '\0';
        i = 0;
        uart_message_ready = 1;
        // uart_transmit('\n');
        // uart_transmit('A');
        uart_print((char *)input);
        stirLEDfromUART(input);
    }
}

uint8_t get_uart_message_ready(void)
{
    return uart_message_ready;
}