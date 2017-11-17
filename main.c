#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "softuart.h"
#include <util/delay.h>
#include <stdbool.h>

int main(void)
{

    // init PWM LED output for stripes
    //PD3 = B
    //PB3 = G
    //PB1 = R
    DDRD  = 0b01101000;   // PD3 PD5 and PD6 outputs
    DDRB  = 0b00001010;   // PB1 PB2 are outputs
 
    
    // input buttons on port B
    //DDRB = 0x00;        //Port B are all inputs
    //PORTB = 0xFF;       //enable all pull-ups

    // enable timer for PWM
    //TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM01) | _BV(WGM00);      // Non inverting mode on OC0A and OC0B, Mode = Mode 3 FAST PWM
    TCCR0B = _BV(CS00);                                                // No prescaling
 
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

	for (;;) {
		if ( softuart_kbhit() ) {
			c = softuart_getchar();
			switch(c) {
				case(0x31):
					pwm_r = 0xFF;
					pwm_g = 0x00;
					pwm_b = 0x00;
    					TCCR1A |= _BV(COM1A1) | _BV(WGM10);
					TCCR2A &= (0<<COM2A1) & (0<<COM2B1);
					softuart_puts_P("OK\n");
					break;
				case(0x34):
					pwm_r = 0x00;
					pwm_g = 0x00;
					pwm_b = 0x00;
					TCCR1A &= (0<<COM1A1);
					TCCR2A &= (0<<COM2A1) & (0<<COM2B1);
					softuart_puts_P("OK\n");
					break;
				case(0x35):
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

		// set LED pwm values if needed
		OCR2A=pwm_g;
		OCR2B=pwm_b;
		OCR1A=pwm_r;
	}
	
	return 0; /* never reached */
}


/*

       //OCR2A = (unsigned char)(150*rgb.blue);
 
        //Uncomment and adjust the values below to adjust white balance, then transfer the value above
        //# if SW0 is pressed show pattern 1
        if((PINB & 0b00010000)==0) {
          OCR0A=pwm;
          OCR0B=pwm;
          OCR2B=pwm;
        } else {
          OCR0A=pwm_r;
          OCR0B=pwm_g;
          OCR2B=pwm_b;
        }

        pwm += up ? 1 : -1;
        if (pwm == 0xff)
          up = false;
        else if (pwm == 0x00)
          up = true;

        pwm_r += up_r ? 1 : -1;
        if (pwm_r == 0xff)
          up_r = false;
        else if (pwm_r == 0x00)
          up_r = true;

        pwm_g += up_g ? 2 : -2;
        if (pwm_g == 0xff)
          up_g = false;
        else if (pwm_g == 0x00)
          up_g = true;

        pwm_b += up_b ? 3 : -3;
        if (pwm_b == 0xff)
          up_b = false;
        else if (pwm_b == 0x00)
          up_b = true;

      _delay_ms(10);
*/
