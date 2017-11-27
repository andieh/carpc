#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "softuart.h"
#include <util/delay.h>
#include <stdbool.h>

#define LED_DIM_SPEED 5

void setR(uint8_t color, bool force) {
    //set led dimm speed
    uint8_t step = LED_DIM_SPEED;
    
    // get current counter value
    uint8_t current = OCR1A;

    // value is current, do nothing
    if (current == color) {
		//softuart_puts_P("value already set\n");
        return;
    }   

    if (current == 0 && color > 0) {
		//softuart_puts_P("enable pwm\n");
        // enable PWM
		TCCR1A |= _BV(COM1A1) | _BV(WGM10);
    	//TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
    }

    uint8_t newValue = color;
    // force to set value
    if (!force) {
		//softuart_puts_P("interpolate\n");
        if (newValue <= (current-step)) {
		    softuart_puts_P("decrease value\n");
            newValue = current - step;
        }
        else if (newValue >= (current+step)) {
		    softuart_puts_P("increase value\n");
            newValue = current + step;
        }
        else
            newValue = color;
    }

    if (newValue > 0xFF) 
        newValue = 0XFF;
    if (newValue <= 0x00) {
		//softuart_puts_P("disable pwm\n");
        // disable PWM
        OCR1A = 0x00;
        TCCR1A &= (0<<COM1A1);
        //TCCR2A &= (0<<COM2A1) & (0<<COM2B1);
        return;
    }
    OCR1A = newValue;
}

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
    TCCR1A |= _BV(COM1A1) | _BV(WGM10);
    TCCR1B |= _BV(CS10) | _BV(WGM12);

    TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);      // Non inverting mode on OC2A, Mode = Mode 3 FAST PWM
    TCCR2B = _BV(CS20);                                                // No prescaling

    uint8_t pwm_r = 0x00; // led off R
    uint8_t pwm_g = 0x00; // led off G
    uint8_t pwm_b = 0x00; // led off B
	TCCR1A &= (0<<COM1A1);
	TCCR2A &= (0<<COM2A1) & (0<<COM2B1);
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
				case(0x31): // "1"
                    //setR(0xFF, false);
                    pwm_r = 0x0F;
					/*pwm_r = 0xFF;
					pwm_g = 0x00;
					pwm_b = 0x00;
    				TCCR1A |= _BV(COM1A1) | _BV(WGM10);
					TCCR2A &= (0<<COM2A1) & (0<<COM2B1);*/
					softuart_puts_P("OK\n");
					break;
                case(0x32):
                    //setR(0x00, false);
                    pwm_r = 0x00;
					softuart_puts_P("OK\n");
					TCCR2A &= (0<<COM2A1) & (0<<COM2B1);
                    break;
				case(0x34): // "4"
					pwm_r = 0x00;
					pwm_g = 0x00;
					pwm_b = 0x00;
					TCCR1A &= (0<<COM1A1);
					TCCR2A &= (0<<COM2A1) & (0<<COM2B1);
					softuart_puts_P("OK\n");
					break;
				case(0x35): // "5"
					pwm_r = 0xFF;
					pwm_g = 0xFF;
					pwm_b = 0xFF;
					TCCR1A |= _BV(COM1A1) | _BV(WGM10);
    					TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);      // Non inverting mode on OC2A, Mode = Mode 3 FAST PWM
					softuart_puts_P("OK\n");
					break;

				default:
					softuart_puts_P("UNKNOWN\n");
					break;
			}
			/*softuart_puts_P("Hello\n");
			pwm_g = 0xFF;
			pwm_r = 0xFF;
			pwm_b = 0xFF;*/
		}

        if((PINB & 0b00010000)==0) {
            set_r = 0xFF;
            set_g = 0xFF;
            set_b = 0xFF;
            set_no_dim = true;
        } else {
            set_r = pwm_r;
            set_g = pwm_g;
            set_b = pwm_b;
            set_no_dim = no_dim;
        }

		// set LED pwm values if needed
		OCR2A=set_g;
		OCR2B=set_b;
		//OCR1A=pwm_r;
        setR(set_r, set_no_dim);
	}
	
	return 0; /* never reached */
}


