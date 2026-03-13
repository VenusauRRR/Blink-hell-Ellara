#include "../include/uart.h"
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../include/definition.h"
#include "string.h"

#define F_CPU 16000000UL

static volatile char input[20];
static volatile uint8_t i = 0;

LGT_STATE led_stir_choice;
char *stir_mode;
char *stir_color;

void splitString(char *input)
{
    stir_mode = strtok(input, " ");
    stir_color = strtok(NULL, " ");
}

void stirLEDfromUART(const char *input)
{
    splitString(input);

    if (strcmp(stir_mode, "disable") == 0)
    {
        led_stir_choice = OFF;
    }
    else if (strcmp(stir_mode, "enable") == 0)
    {
        led_stir_choice = ON;
    }
    else if (strcmp(stir_mode, "toggle") == 0)
    {
        led_stir_choice = TOGGLE;
    }

    if (strcmp(stir_color, "red") == 0)
    {
        leds_uartMode.red_st = led_stir_choice;
    }
    else if (strcmp(stir_color, "green") == 0)
    {
        leds_uartMode.green_st = led_stir_choice;
    }
    else if (strcmp(stir_color, "blue") == 0)
    {
        leds_uartMode.blue_st = led_stir_choice;
    }
    else if (strcmp(stir_color, "white") == 0)
    {
        leds_uartMode.white_st = led_stir_choice;
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
    sys_mode = UART;
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