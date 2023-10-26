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
    send_signal();
    //button_pressed = 1;
}

int is_button_pressed()
{
    return PINB & (1 << BUTTON_PIN);
}

int main()
{
    // Set output pin as output.
    DDRB |= (1 << OUTPUT_PIN);

    // Set output pin low.
    PORTB &= ~(1 << OUTPUT_PIN);

    // Enable the INT0 interrupt.
    GIMSK |= (1 << INT0);
    MCUCR |= (1 << ISC00) | (1 << ISC01);

    // Set the button pin as an input.
    DDRB &= ~(1 << BUTTON_PIN);

    // Enable interrupts.
    sei();

    // Set sleep mode to power down.
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);

    // Infinite loop.
    while (1)
    {
        sleep_mode();
        _delay_ms(2000);

        // // Wait for button press.
        // while (!button_pressed)
        // {
        //     // Sleep until an interrupt occurs.
        //     sleep_mode();
        // }

        // // Wait for button release.
        // uint8_t sleep_count = 0;
        // do
        // {
        //     _delay_ms(100);
        //     sleep_count++;
        // } while (sleep_count < 10 && is_button_pressed());

        // // Send the signal...
        // if (sleep_count < 10)
        // {
        //     // ...immediately.
        //     send_signal();
        // }
        // else
        // {
        //     // ...after 5 seconds.
        //     _delay_ms(5000);
        //     send_signal();
        // }

        // // Clear the button pressed flag.
        // button_pressed = 0;
    }
}