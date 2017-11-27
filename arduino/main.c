#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "softuart.h"
#include <util/delay.h>
#include <stdbool.h>
#include "led.h"

int main(void)
{
    // init PWM LED output for stripes
    //PD3 = B
    //PB3 = G
    //PB1 = R
    DDRD  = 0b00001000;   // PD3 PD5 and PD6 outputs
    DDRB  = 0b00001010;   // PB1 PB2 are outputs
 
    
    // input buttons on port B
    //DDRB = 0x00;        //Port B are all inputs
    //PORTB = 0xFF;       //enable all pull-ups
    PORTB = 0b00010000; // pull-up for PB4
    

    // enable timer for PWM
    TCCR1A |= _BV(COM1A1) | _BV(COM1A0) | _BV(WGM10);
    TCCR1B |= _BV(CS10) | _BV(WGM12);
    //Set on match, clear on TOP
    //TCCR1A  = ((1 << COM1A1) | (1 << COM1A0) | (1 << WGM10));
    // Phase + Frequency Correct PWM, Fcpu speed
    //TCCR1B  = ((1 << CS10) | (1 << WGM12));

    // Inverting mode on OC2A, Mode = Mode 3 FAST PWM
    TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(COM2A0) | _BV(COM2B0) | _BV(WGM21) | _BV(WGM20);      
    // No prescaling
    TCCR2B = _BV(CS20);                                                

    uint8_t pwm_r = 0xFF; // led off R
    uint8_t pwm_g = 0xFF; // led off G
    uint8_t pwm_b = 0xFF; // led off B
	char c;

	softuart_init();
	softuart_turn_rx_on(); /* redundant - on by default */

	sei();

    bool no_dim = false; // dims the led if requested, otherwise set value instantly
    bool set_no_dim = no_dim;
    uint8_t set_r = pwm_r;
    uint8_t set_g = pwm_g;
    uint8_t set_b = pwm_b;

    softuart_puts_P("here we go!\n");

	for (;;) {
		if ( softuart_kbhit() ) {
			c = softuart_getchar();
			switch(c) {
				case(0x31): // "1" = all LED on
					pwm_r = 0x00;
					pwm_g = 0x00;
					pwm_b = 0x00;
					softuart_puts_P("OK\n");
					break;
                case(0x32): // "2" = all LED off
                    pwm_r = 0xFF;
                    pwm_g = 0xFF;
                    pwm_b = 0xFF;
					softuart_puts_P("OK\n");
                    break;
				case(0x33): // "3" = Red light
					pwm_r = 0x00;
					pwm_g = 0xFF;
					pwm_b = 0xFF;
					softuart_puts_P("OK\n");
					break;
				case(0x34): // "4" = Green light
					pwm_r = 0xFF;
					pwm_g = 0x00;
					pwm_b = 0xFF;
					softuart_puts_P("OK\n");
					break;
				case(0x35): // "5" = Blue light
					pwm_r = 0xFF;
					pwm_g = 0xFF;
					pwm_b = 0x00;
					softuart_puts_P("OK\n");
					break;


				default:
					softuart_puts_P("UNKNOWN\n");
					break;
			}
		}

        if((PINB & 0b00010000)==0) {
            set_r = 0x00;
            set_g = 0x00;
            set_b = 0x00;
            set_no_dim = true;
        } else {
            set_r = pwm_r;
            set_g = pwm_g;
            set_b = pwm_b;
            set_no_dim = no_dim;
        }

		// set LED pwm values if needed
        setRGB(set_r, set_g, set_b, set_no_dim);
	}
	
	return 0; /* never reached */
}


