#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "softuart.h"
#include <util/delay.h>
#include <stdbool.h>
#include "led.h"
 

/* Einfache Funktion zum Entprellen eines Tasters */
inline uint8_t debounce(volatile uint8_t *port, uint8_t pin)
{
    if ( (*port & (1 << pin)) )
    {
        softuart_puts_P("entprellen!!\n");
        /* Pin wurde auf Masse gezogen, 100ms warten   */
        _delay_ms(50);   // Maximalwert des Parameters an _delay_ms 
        _delay_ms(50);   // beachten, vgl. Dokumentation der avr-libc
        if ( !(*port & (1 << pin)) )
        {
            /* Anwender Zeit zum Loslassen des Tasters geben */
            _delay_ms(50);
            _delay_ms(50); 
            return 1;
        }
    }
    return 0;
}

uint16_t adc_read(uint8_t channel) {
   uint8_t i;
   uint16_t result = 0;
   
   // ADEN wird 1 gesetzt = ADC aktivieren
   // ADPS2 = 1 und ADPS1 = 1  beduetet:
   // Teilungsfaktor ist = 64
   // Quelle: http://www.mikrocontroller.net/articles/AVR-GCC-Tutorial/Analoge_Ein-_und_Ausgabe#Messen_eines_Widerstandes
   ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1);

   // ADMUX wählt den zu messenden Kanal
   // "channel" des Multiplexers waehlen
   // REFS1 = 1 und REGS0 = 1 Interne Referenzspannung verwenden (also 2,56 V)
   // REFS1 = 0 und REGS0 = 0 Externe Regerenzspannung verwenden ()
   ADMUX = channel | (1<<REFS1) | (1<<REFS0);
   
   // ADCSRA initialisiert den ADC und definiert dessen Funktion
   // Den ADC initialisieren und einen sog. Dummyreadout machen
   // ADSC bleibt solange 1 bist die Konvertierung abgeschlossen ist
   ADCSRA |= (1<<ADSC);
   while(ADCSRA & (1<<ADSC));
   
   // Jetzt 3x die analoge Spannung and Kanal channel auslesen
   // und dann Durchschnittswert ausrechnen.
   for(i=0; i<3; i++) {
      // Eine Wandlung
      ADCSRA |= (1<<ADSC);
      // Auf Ergebnis warten...
      while(ADCSRA & (1<<ADSC));
      
      result += ADCW;
   }
   
   // ADC wieder deaktivieren
   ADCSRA &= ~(1<<ADEN);
   
   result /= 3;
   
   return result;
}

int main(void)
{
    // init PWM LED output for stripes
    // PD3 = B
    // PB3 = G
    // PB1 = R
    // LED for buttons on 
    // PD7 = radio
    DDRD  = 0b10001000;   // PD3 outputs
    DDRB  = 0b00001010;   // PB1 PB2 outputs
 
    // input buttons on port B
    //DDRB = 0x00;        // Port B are all inputs
    //PORTB = 0xFF;       // enable all pull-ups

    //DDRB &= ~( 1 << PB0 );        /* PIN PB0 auf Eingang Taster)  */
    //PORTB = 0b00010001;   // pull-up for PB0, PB4
    PORTB |= ( 1 << PB0 );        // Pullup Innenlicht
    PORTB |= ( 1 << PB4 );        // Pullup Radio
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

    softuart_puts("here we go!\n");

	for (;;) {

        // check if the doors are open
        if (adc_read(PC5) > door_voltage_threshold)
            doors_open = false;
        else
            doors_open = true;

        // check if someone pressed the button for lights
        if ((PINB & 0b00010000)==0)
            lights_on = true;
        else 
            lights_on = false;

        // check if button for RADIO is pressed
        //if ((PINB & 0b00000001)==1) { // button pressed
        if (debounce(&PINB, PB0)) {
            radio_on = !radio_on;
        }

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
        setRGB(set_r, set_g, set_b, set_no_dim);

        /*char str[16];
        itoa(adc_read(ADC_PIN), str, 10);
        str[5] = '\0';
        _delay_ms(50);
        softuart_puts(str);
        softuart_puts_P("\n");*/

        // turn on radio button LED
        if (radio_on) {
            PORTD |= (1 << PD7); // output high
        } else {
            PORTD &= ~(1 << PD7); // output low
        }

	}
	
	return 0; /* never reached */
}


