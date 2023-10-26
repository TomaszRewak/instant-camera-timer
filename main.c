#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>

#define BUTTON_PIN PB1
#define OUTPUT_PIN PB0

volatile uint8_t button_pressed = 0;

void send_signal()
{
    PORTB |= (1 << OUTPUT_PIN);
    _delay_ms(1000);
    PORTB &= ~(1 << OUTPUT_PIN);
}

ISR(INT0_vect)
{
    button_pressed = 1;
}

int main()
{
    // Set output pin as output.
    DDRB |= (1 << OUTPUT_PIN);

    // Set output pin low.
    PORTB &= ~(1 << OUTPUT_PIN);

    // Disable unused peripherals for power saving.
    ADCSRA &= ~(1 << ADEN); // Disable ADC.

    // // Add PCINT1 interrupt.
    // GIMSK |= (1 << PCIE);
    // PCMSK |= (1 << PCINT1);

    // Enable the INT0 interrupt.
    GIMSK |= (1 << INT0);
    MCUCR |= (1 << ISC00) | (1 << ISC01);

    // Set the button pin as an input.
    DDRB &= ~(1 << BUTTON_PIN);

    // Enable interrupts.
    sei();

    // Set sleep mode to power down.
    set_sleep_mode(SLEEP_MODE_IDLE);

    // Infinite loop.
    while (1)
    {
        sleep_mode();

        if (button_pressed)
        {
            button_pressed = 0;
            send_signal();
        }
    }
}