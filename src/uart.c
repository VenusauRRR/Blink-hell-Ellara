#include "../include/uart.h"
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../include/definition.h"
#include "string.h"

#define F_CPU 16000000UL

static volatile char input[20];
static volatile uint8_t i = 0;

uint8_t led_stir_mode = 0;
LED_STIR_STATE led_stir_choice = DISABLE;
LED_STATE led_stir_color = 0;
char *stir_mode;
char *stir_color;

void splitString(char *input)
{
    stir_mode = strtok(input, " ");
    stir_color = strtok(NULL, " ");
}

void stirLEDfromUART(const char *input)
{
    rotor_state_idx = 0;
    led_stir_mode = 1;
    uart_print_uint16(led_stir_mode);
    splitString(input);

    if (strcmp(stir_mode, "off") == 0)
    {
        led_stir_choice = DISABLE;
    }
    else if (strcmp(stir_mode, "on") == 0)
    {
        led_stir_choice = ENABLE;
    }
    else if (strcmp(stir_mode, "toggle") == 0)
    {
        led_stir_choice = TOGGLE;
    }

    if (strcmp(stir_color, "red") == 0)
    {
        led_stir_color = LED_RED;
    }
    else if (strcmp(stir_color, "green") == 0)
    {
        led_stir_color = LED_GREEN;
    }
    else if (strcmp(stir_color, "blue") == 0)
    {
        led_stir_color = LED_BLUE;
    }
    else if (strcmp(stir_color, "white") == 0)
    {
        led_stir_color = LED_WHITE;
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
    char a = UDR0;
    if (a != '\n' && a != '\r')
    {
        if (i < 19)
        {
            input[i] = a;
            i++;
        }
        else
        {
            input[20] = '\0';
            i = 0;
            uart_print((char *)input);
            stirLEDfromUART(input);
        }
    }
    else
    {
        input[i] = '\0';
        i = 0;
        uart_print((char *)input);
        stirLEDfromUART(input);
    }
}