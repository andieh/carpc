/*
 *
 *            ----------------------------------
 *            |           ARDUINO UNO          |
 *            |            --------            |
 *            |            |      | PC5     A4 |
 *            |            |      | PC4     A5 |
 *            |            |      |       AREF |
 *            |            |      |        GND |
 *            |            |      | PB5    D13 | < QH (buttons) 
 *            | NC         |ATMEGA| PB4    D12 | > CLK (buttons)
 *            | IOREF      | 328P | PB3    D11 | > G channel (PWM)
 *            | 3V3        |      | PB2    D10 | < Light button (back)
 *            | 5V         |      | PB1     D9 | > R channel (PWM)
 *            | GND        |      | PB0     D8 | > SHLD (buttons)
 *            | GND        |      |            |
 *            | Vin        |      | PD7     D7 | > 12V radio enable 
 *            |            |      | PD6     D6 | > CLK (LED shift register)
 *            | A0     PC0 |      | PD5     D5 | > Data (LEDs shift register)
 *            | A1     PC1 |      | PD4     D4 | > TX (softuart RPi)
 *            | A2     PC2 |      | PD3     D3 | > B channel (PWM)
 *            | A3     PC3 |      | PD2     D2 | < RX (softuart RPi)
 *            | A4     PC4 |      | PD1     D1 | > TX (EJ22)
 *            | A5     PC5 |      | PD0     D0 | < RX (EJ22)
 *            |            --------            |
 *            |---------------------------------
 *
 *  t.b.d.:
 *  - output: Radio enable 
 *  - output: 5V enable
 *  - input: doors open?
 *  - input: door locked?
 *  - output: alarm horn
 *  - ESP8622: - connection to the ESP8622??? RX / TX 
 *             - enable / disable?
 */


#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "softuart.h"
#include <util/delay.h>
#include <stdbool.h>
#include "led.h"
#include "buttons.h"

#include <stdlib.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/sfr_defs.h>


// read of LEDs 
// 2 pins for setting up to unlimited number 
// of LEDs. in our case up to 8 (two shift 
// registers).
// PD5 = digital 5
// PD6 = digital 6
#define CP_PIN  PORTD5 // clock
#define CP_PORT PORTD
#define DP_PIN  PORTD6 // data
#define DP_PORT PORTD 
// definition of the position of led to set
#define LED_PIN_RADIO  0 // enable radio
#define LED_PIN_LIGHTS 1 // enable light inside
#define LED_PIN_FIVE   2 // 5 volt rail enabled?
#define LED_PIN_ALARM  3 // enable / disable alarm
#define LED_PIN_ELSE1  4 // t.b.d.
#define LED_PIN_ELSE2  5 // t.b.d.
#define LED_PIN_POWER  6 // shows a 220V connection
#define LED_PIN_VISU   7 // display only led (tbd.)  
 
// button connection definition
// 3 pins for reading up to 8 buttons
// PB0 = digital 8
// PB4 = digital 12 (miso)
// PB5 = digital 13 (SCK) 
#define SHLD      PORTB0 
#define CLK       PORTB4 
#define QH        PORTB5 
#define DDR_SHLD  DDRB
#define PORT_SHLD PORTB
#define DDR_CLK   DDRB
#define PORT_CLK  PORTB
#define DDR_QH    DDRB
#define PORT_QH   PORTB
#define PIN_QH    PINB /* This is the only input. */

// outputs 
#define RADIO_OUT_PORT   PORTD
#define RADIO_OUT_PIN    PORTD7

void init_buttons(void) {
    /* In input mode, when pull-up is enabled, default state of pin becomes ’1′. So even if */
    /* you don’t connect anything to pin and if you try to read it, it will read as 1. Now, */
    /* when you externally drive that pin to zero(i.e. connect to ground / or pull-down),   */
    /* only then it will be read as 0. */
    /* Enable pullups by setting bits in PORT. Default state is now high. */
    DDR_SHLD |= (_BV(SHLD));  /* Output */
    DDR_CLK |= (_BV(CLK));  /* Output */
    DDR_QH &= ~(_BV(QH)); /* Input  */
}

uint8_t digital_read(int input_register, int pin) {
    return bit_is_set(input_register, pin) != 0 ? 1 : 0;
}

/* You could use cbi ie &= ~ or sbi ie |= but this makes code more readable. */
void digital_write(volatile uint8_t *data_port, uint8_t pin, uint8_t value) {
    if (0 == value) {
        *data_port &= ~(_BV(pin));
    } else {
        *data_port |= _BV(pin);
    }
}

void set_leds(uint8_t leds) {
  uint8_t MAX_LEDS = 8; // number of LEDs connected
  for (uint8_t i = MAX_LEDS; i > 0; --i) {
    if (bit_is_set(leds,(i-1)))
      DP_PORT |= (_BV(DP_PIN));
    else
      DP_PORT &= ~(_BV(DP_PIN));
    CP_PORT |= (_BV(CP_PIN));
    CP_PORT &= ~(_BV(CP_PIN));
  }
}

uint8_t set_led(uint8_t leds, bool cond, uint8_t pin) {
  if (cond) 
    leds |= ( 1 << pin);
  else 
    leds &= ~(1 << pin);
  return leds;
}

int main(void)
{
    // init PWM LED output for stripes
    // PD3 = B
    // PB3 = G
    // PB1 = R
    // LED for buttons on 
    // PD7 = radio
    DDRD  = 0b11101000;   // PD3 outputs
    DDRB  = 0b00001010;   // PB1 PB3 PB5 outputs

    init_buttons();
    
    uint8_t register_value;
    uint8_t pin_value;
    bool radio = false;
    bool light = false;
    bool door_switch = false;
    bool five = false;
    bool alarm = false;
    bool else1 = false;
    bool else2 = false;
    bool twotwenty = true;
    bool unknown = true;

    // input buttons on port B
    //DDRB = 0x00;        // Port B are all inputs
    //PORTB = 0xFF;       // enable all pull-ups

    //DDRB &= ~( 1 << PB0 );        /* PIN PB0 auf Eingang Taster)  */
    //PORTB = 0b00010001;   // pull-up for PB0, PB4
    //PORTB |= ( 1 << PB2 );        // Pullup Innenlicht
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
    
    // init softuart transmission
    char c;
    softuart_init();
    softuart_turn_rx_on(); /* redundant - on by default */
    sei();
    
    // clear LEDs
    uint8_t leds = 0b00000000;
    set_leds(leds); 

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

    //static bool animateState = false;
    static bool animateState[3] = {false, false, false};
    static uint8_t animatePos = 1;

    uint16_t animateCnt = 1;

    // lets start everything
    softuart_puts("here we go!\n");

	  for (;;) {

      // check if the doors are open
      /*if (adc_read(PC5) > door_voltage_threshold)
          doors_open = false;
      else
          doors_open = true;*/

      /* Read in parallel input by setting SH/LD low. */
      digital_write(&PORT_SHLD, SHLD, 0); 
      /* Freeze data by setting SH/LD high. When SH/LD is high data enters */
      /* to reqisters from SER input and shifts one place to the right     */
      /* (Q0 -> Q1 -> Q2, etc.) with each positive-going clock transition. */
      digital_write(&PORT_SHLD, SHLD, 1);
      
      bool inlight = (light || doors_open);
      uint8_t new_leds = 0b00000000;
      new_leds = set_led(new_leds, radio, LED_PIN_RADIO);
      new_leds = set_led(new_leds, inlight, LED_PIN_LIGHTS);
      new_leds = set_led(new_leds, five,  LED_PIN_FIVE);
      new_leds = set_led(new_leds, alarm, LED_PIN_ALARM);
      new_leds = set_led(new_leds, else1, LED_PIN_ELSE1);
      new_leds = set_led(new_leds, else2, LED_PIN_ELSE2);
      new_leds = set_led(new_leds, twotwenty, LED_PIN_POWER);
      new_leds = set_led(new_leds, unknown, LED_PIN_VISU);
      if (new_leds != leds) {
        set_leds(new_leds);
        leds = new_leds;
      }

      uint8_t delayAfterPress = 200;
      register_value = 0;
      for(uint8_t i=0; i<8; i++) {
        pin_value = digital_read(PIN_QH, QH);  
        register_value |= (pin_value << ((8 - 1) - i));          
        if (i==2) {
          if (!five && pin_value) {
            _delay_ms(delayAfterPress);
            five = true;
          } else if (five && pin_value) {
            _delay_ms(delayAfterPress);
            five = false;
          }
        }
        
        else if (i==3) {
          if (!else1 && pin_value) {
            _delay_ms(delayAfterPress);
            else1 = true;
          } else if (else1 && pin_value) {
            _delay_ms(delayAfterPress);
            else1 = false;
          }
        }

        else if(i==4) {
          if (!alarm && pin_value) {
            _delay_ms(delayAfterPress);
            alarm = true;
          } else if (alarm && pin_value) {
            _delay_ms(delayAfterPress);
            alarm = false;
          }
        }
        
        else if (i==5) {
          if (!radio && pin_value) {
            _delay_ms(delayAfterPress);
            radio = true;
          } else if (radio && pin_value) {
            _delay_ms(delayAfterPress);
            radio = false;
          }
        }
        
        else if (i==6) {
          if (!else2 && pin_value) {
            _delay_ms(delayAfterPress);
            else2 = true;
          } else if (else2 && pin_value) {
            _delay_ms(delayAfterPress);
            else2 = false;
          }
        }
        
        else if (i==7) {
          if (!light && pin_value) {
            _delay_ms(delayAfterPress);
            light = true;
          } else if (light && pin_value) {
            _delay_ms(delayAfterPress);
            light = false;
          }
        }
        // Pulse clock input (CP) LOW-HIGH to read next bit.
        digital_write(&PORT_CLK, CLK, 0);
        digital_write(&PORT_CLK, CLK, 1);
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
      if (inlight) {
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

      if (radio)
        RADIO_OUT_PORT |= (_BV(RADIO_OUT_PIN));
      else
        RADIO_OUT_PORT &= ~(_BV(RADIO_OUT_PIN));

      /*char str[16];
      itoa(adc_read(ADC_PIN), str, 10);
      str[5] = '\0';
      _delay_ms(50);
      softuart_puts(str);
      softuart_puts_P("\n");*/

	} // end for
	
	return 0; /* never reached */
}


