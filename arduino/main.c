/*
 *
 *            ----------------------------------
 *            |           ARDUINO UNO          |
 *            |            --------            |
 *            |            |      | PC5     A4 |
 *            |            |      | PC4     A5 |
 *            |            |      |       AREF |
 *            |            |      |        GND |
 *            |            |      | PB5    D13 |
 *            | NC         |ATMEGA| PB4    D12 | 
 *            | IOREF      | 328P | PB3    D11 | > G channel (PWM)
 *            | 3V3        |      | PB2    D10 | < Light button (back)
 *            | 5V         |      | PB1     D9 | > R channel (PWM)
 *            | GND        |      | PB0     D8 |
 *            | GND        |      |            |
 *            | Vin        |      | PD7     D7 |
 *            |            |      | PD6     D6 |
 *            | A0     PC0 |      | PD5     D5 |
 *            | A1     PC1 |      | PD4     D4 | > TX (softuart RPi)
 *            | A2     PC2 |      | PD3     D3 | > B channel (PWM)
 *            | A3     PC3 |      | PD2     D2 | < RX (softuart RPi)
 *            | A4     PC4 |      | PD1     D1 | > TX (EJ22)
 *            | A5     PC5 |      | PD0     D0 | < RX (EJ22)
 *            |            --------            |
 *            |---------------------------------
 *
 *
 *
 *
 *
 *
 *
 */


#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "softuart.h"
#include <util/delay.h>
#include <stdbool.h>
#include "led.h"
#include "buttons.h"

int main(void)
{
    // init PWM LED output for stripes
    // PD3 = B
    // PB3 = G
    // PB1 = R
    // LED for buttons on 
    // PD7 = radio
    DDRD  = 0b00001000;   // PD3 outputs
    DDRB  = 0b00001010;   // PB1 PB3 PB5 outputs
 
    // input buttons on port B
    //DDRB = 0x00;        // Port B are all inputs
    //PORTB = 0xFF;       // enable all pull-ups

    //DDRB &= ~( 1 << PB0 );        /* PIN PB0 auf Eingang Taster)  */
    //PORTB = 0b00010001;   // pull-up for PB0, PB4
    PORTB |= ( 1 << PB2 );        // Pullup Innenlicht
    //PORTB |= ( 1 << PB4 );        // Pullup Radio
    //PORTC |= ( 1 << PC5 );        // Pullup Tuerkontakt

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

    // Enable the ADC
    uint16_t door_voltage_threshold = 200;
    ADCSRA |= _BV(ADEN);

    uint8_t pwm_r = 0xFF; // led off R
    uint8_t pwm_g = 0xFF; // led off G
    uint8_t pwm_b = 0xFF; // led off B
	char c;

	softuart_init();
	softuart_turn_rx_on(); /* redundant - on by default */

	sei();

    // PWM and stuff for LED stripe
    bool no_dim = false; // dims the led if requested, otherwise set value instantly
    bool set_no_dim = no_dim;
    uint8_t set_r = pwm_r;
    uint8_t set_g = pwm_g;
    uint8_t set_b = pwm_b;

    // radio on button
    bool radio_on = false;

    // doors are open
    bool doors_open = false;

    // light button pressed
    bool lights_on = false;
    
    //static bool animateState = false;
    static bool animateState[3] = {false, false, false};
    static uint8_t animatePos = 1;

    uint16_t animateCnt = 1;

    // lets start everything
    softuart_puts("here we go!\n");

	  for (;;) {
      // check if the doors are open
      if (adc_read(PC5) > door_voltage_threshold)
          doors_open = false;
      else
          doors_open = true;

      // check if someone pressed the button for lights
      if ((PINB & 0b00000100)==0)
          lights_on = true;
      else 
          lights_on = false;
      
      // UART communication with the raspberry pi
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

      // enable LED stripes if requested
      if (lights_on || doors_open) {
          set_r = 127; //0x00;
          set_g = 127; //0x00;
          set_b = 127; //0x00;
          set_no_dim = true;
      } else {
          set_r = pwm_r;
          set_g = pwm_g;
          set_b = pwm_b;
          set_no_dim = no_dim;
      }
      setRGB(set_r, set_g, set_b, set_no_dim);

      /*char str[16];
      itoa(adc_read(ADC_PIN), str, 10);
      str[5] = '\0';
      _delay_ms(50);
      softuart_puts(str);
      softuart_puts_P("\n");*/

	} // end for
	
	return 0; /* never reached */
}


