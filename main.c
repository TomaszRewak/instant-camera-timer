#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>

#define BUTTON_PIN PB1
#define OUTPUT_PIN PB0

#define SIGNAL_DURATION_MS 300
#define SHORT_BUTTON_PRESS_TIME_MS 1500
#define BUTTON_PROBING_INTERVAL_MS 100
#define LONG_BUTTON_PRESS_SIGNAL_DELAY_MS 10000

volatile uint8_t button_pressed = 0;

void send_signal()
{
    PORTB &= ~(1 << OUTPUT_PIN);
    _delay_ms(SIGNAL_DURATION_MS);
    PORTB |= (1 << OUTPUT_PIN);
}

short is_button_pressed()
{
    return !(PINB & (1 << BUTTON_PIN));
}

short is_long_button_press()
{
    int delay = 0;

    do
    {
        _delay_ms(BUTTON_PROBING_INTERVAL_MS);
        delay += BUTTON_PROBING_INTERVAL_MS;
    } while (is_button_pressed() && delay < SHORT_BUTTON_PRESS_TIME_MS);

    return delay >= SHORT_BUTTON_PRESS_TIME_MS;
}

short wait_for_button_release()
{
    while (is_button_pressed())
    {
        _delay_ms(BUTTON_PROBING_INTERVAL_MS);
    }
}

ISR(INT0_vect)
{
    button_pressed = 1;
}

int main()
{
    // Set output pin as output.
    DDRB |= (1 << OUTPUT_PIN);

    // Set output pin high.
    PORTB |= (1 << OUTPUT_PIN);

    // Disable unused peripherals for power saving.
    ADCSRA &= ~(1 << ADEN); // Disable ADC.

    // Enable the INT0 interrupt.
    GIMSK |= (1 << INT0);
    MCUCR |= (1 << ISC01);

    // Set the button pin as an input.
    DDRB &= ~(1 << BUTTON_PIN);

    // Enable interrupts.
    sei();

    // Set sleep mode to idle.
    set_sleep_mode(SLEEP_MODE_IDLE);

    // Infinite loop.
    while (1)
    {
        sleep_mode();

        if (button_pressed)
        {
            if (is_long_button_press())
            {
                wait_for_button_release();

                _delay_ms(LONG_BUTTON_PRESS_SIGNAL_DELAY_MS);
            }

            send_signal();

            button_pressed = 0;
        }
    }
}