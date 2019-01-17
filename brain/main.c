/**************************
 * PUFFIS BRAIN, Car-Control v.0.2
 *
 * PORTB
 * PB0  D8   >  touch_pwm   Dims the LEDs on the Touch board 
 * PB1  D9   >  sig_horn    For alarm purpurses, enable horn. 
 *                          \Todo: own board for power control?
 * PB2  D10  >  sig_lock    Locks the car using a N-Transistor
 * PB3  D11  >  NEW: sig_unlock
 *                   replaced sig_220v    check if 220V are available. 5V is read
 *                          by the optocoppler
 * PB4  D12  >  sig_eis     Enables the 12V P-Transistor output for 
 *                          the EisEx.
 * PB5  D13  >  sig_5v      Enables 12V for the 5V rail.
 *
 * PORTC
 * PC0  A0   >  NEW: 220_led
 *                   replaced analog_light  measure voltage for dimming touch LEDs
 * PC1  A1   <  sig_door      doors were opened (ground enabled)
 * PC2  A2   >  sig_radio     enabled radio in the front
 * PC3  A3   <  sig_locked    high if car is locked
 * PC4  A4   >  NEW: SCL      
 *                   replaced 220_led       indicates 220V are available 
 * PC5  A5   >  NEW: SCA      
 *                   replaced SCL i2c for touch panel
 * PC6  A6   <  NEW: analog_light   
 *                   replaced SDA i2c for touch panel
 * PC7  A7   <  NEW: sig_220v
 *                   replaced sig_unlock    Unlocks the car using a N-Transistor
 *
 * PORTD
 * PD0  RX   <  RX          Receive Data from the Arduino
 * PD1  TX   >  TX          Trasmit Data to the Alarm Arduino
 * PD2  D2   <  soft_RX     Receive Data from the RPI 
 *                          \Todo: implement this!
 * PD3  D3   >  soft TX     Transmit Data to the RPI 
 *                          \Todo: implement this!
 * PD4  D4   >  sig_w       LED Stripe, white LED
 * PD5  D5   >  sig_r       LED Stripe, red LED
 * PD6  D6   >  sig_g       LED Stripe, green LED
 * PD7  D7   >  sig_b       LED Stripe, blue LED
 *
 */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "types.h"
#include "defs.h"
#include <util/delay.h>
#include "i2c.h"
#include "mpr121.h"
#include "stdbool.h"
#define FOSC 8000000
#define MPR121_R 0xB5	// ADD pin is grounded
#define MPR121_W 0xB4	// So address is 0x5A

#define sbi(var, mask)   ((var) |= (uint8_t)(1 << mask))
#define cbi(var, mask)   ((var) &= (uint8_t)~(1 << mask))

#define F_PWM 100                       // PWM-Frequenz in Hz
#define PWM_STEPS 256                   // PWM-Schritte pro Zyklus(1..256)
#define PWM_DDR DDRD                    // Datenrichtungsregister für PWM

// ab hier nichts ändern, wird alles berechnet
#define T_PWM (F_CPU/(F_PWM*PWM_STEPS)) // Systemtakte pro PWM-Takt
#if (T_PWM<(93+5))
    #error T_PWM zu klein, F_CPU muss vergrössert werden oder F_PWM oder PWM_STEPS verkleinert werden
#endif

#define LED_COLOR_WHITE 0
#define LED_COLOR_BLUE 1
#define LED_COLOR_RED 2
#define LED_COLOR_GREEN 3
#define LED_COLOR_TOUCH 4

#define LED_PORT_TOUCH  PORTB
#define LED_PIN_TOUCH   PORTB0
#define LED_PORT_WHITE  PORTD
#define LED_PIN_WHITE   PORTD4
#define LED_PORT_RED    PORTD
#define LED_PIN_RED     PORTD5
#define LED_PORT_GREEN  PORTD
#define LED_PIN_GREEN   PORTD6
#define LED_PORT_BLUE   PORTD
#define LED_PIN_BLUE    PORTD7

#define IN_PORT_DOOR    PORTC
#define IN_PIN_DOOR     PC1
#define IN_PORT_LOCKED  PORTC 
#define IN_PIN_LOCKED   PC3
#define MAX_LOCK_CNT    100

#define IN_LIGHT_SENSE 6 // PC6
#define IN_220_SENSE 7 // PC7
#define LIGHT_THRESH	512

#define OUT_RADIO_PORT   PORTC
#define OUT_RADIO_PIN    PORTC2
#define OUT_EIS_PORT     PORTB
#define OUT_EIS_PIN      PORTB4
#define OUT_5V_PORT      PORTB
#define OUT_5V_PIN       PORTB5
#define OUT_220_PORT     PORTC 
#define OUT_220_PIN      PORTC0
#define OUT_LOCK_PORT    PORTB 
#define OUT_LOCK_PIN     PORTB2
#define OUT_UNLOCK_PORT  PORTB 
#define OUT_UNLOCK_PIN   PORTB3

// includes

#include <stdint.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"
#define UART_BAUD_RATE 9600
#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)

///============Function Prototypes=========/////////////////
char mpr121Read(unsigned char address);
void mpr121Write(unsigned char address, unsigned char data);
void mpr121QuickConfig(void);
void mpr121ViewRegisters(void);
int checkInterrupt(void);
uint8_t set_new_led_color();
void set_output(volatile uint8_t *port, uint8_t pin, bool value);
ISR (TIMER2_COMPA_vect);
void USART_init(void);
unsigned char USART_receive(void);
void USART_send( unsigned char data);
void USART_putstring(char* StringPtr);
uint16_t adc_read(uint8_t adcx);

volatile uint8_t led_color[5];                    // Einstellungen für die einzelnen PWM-Kanäle
int old_led_value = 0;

int TOUCH_RADIO  = 1 << 2;
int TOUCH_EIS    = 1 << 3;
int TOUCH_LIGHT  = 1 << 4;
int TOUCH_5V     = 1 << 5;
int TOUCH_MISC   = 1 << 6;
int TOUCH_LOCK   = 1 << 7;

bool touched_radio = false;
bool touched_eis = false;
bool touched_light = false;
bool touched_5v = false;
bool touched_misc = false;
bool touched_lock = false;

int main(void)
{
  /* Enable the ADC */
  ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));    //16Mhz/128 = 125Khz the ADC reference clock
  ADMUX  |= (1<<REFS0);                //Voltage reference from Avcc (5v)
  ADCSRA |= (1<<ADEN);                //Turn on ADC
  ADCSRA |= (1<<ADSC);
  
  unsigned int c;
  uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) );
  uart_putc('\r');

  /* SOFTPWM STUFF*/
  OCR2A = 62;
  TCCR2A |= (1 << WGM21);
  // Set to CTC Mode
  TIMSK2 |= (1 << OCIE2A);
  //Set interrupt on compare match
  TCCR2B |= (1 << CS21);
  // set prescaler to 64 and starts PWM
  sei();
  // enable interrupts
	
  char touchstatus;
	ioinit();
	i2cInit();
	_delay_ms(100);
	checkInterrupt();
	
  mpr121QuickConfig();
	
	// Setup the GPIO pins to drive LEDs
	mpr121Write(GPIO_EN, 0xFF);	// 0x77 is GPIO enable
	mpr121Write(GPIO_DIR, 0xFF);	// 0x76 is GPIO Dir
	mpr121Write(GPIO_CTRL0, 0xFF);	// Set to LED driver
	mpr121Write(GPIO_CTRL1, 0xFF);	// GPIO Control 1
	mpr121Write(GPIO_CLEAR, 0xFF);	// GPIO Data Clear
	
	// Blink LEDs to begin
	for (int i = 0; i < 5; i++)
	{
		mpr121Write(GPIO_SET, 0xFF);
		_delay_ms(50);
		mpr121Write(GPIO_CLEAR, 0xFF);
		_delay_ms(50);
	}

  int delay=50;
  int state = 0x00;
  int old_new_state = state;
  
  mpr121Write(GPIO_CLEAR, 0xFF);	// Clear all LEDs
  int cnt = 0;
  int touch_cnt = 0;
  int step = 0;
  uint8_t white_fade = 0;


  bool red_on = false;
  // alarm state
  enum alarm_fsm {
    ALARM_OFF = 0, // alarm is turned off 
    ALARM_WAIT,    // wait until alarm is turned on 
    ALARM_FOCUS,   // alarm is wait until released
    ALARM_DELAY,   // wait to shutdown alarm 
    ALARM          // alarm is triggered
  };
  enum alarm_fsm alarm = ALARM_OFF;  // current fsm 
  bool alarm_state = false; // alarm is active, flash lights
  int alarm_cnt = 0; // cnt for alarm
  bool alarm_triggered = false; // alarm over UART was triggered!
  bool alarm_requested = false; // alarm was requested over UART

  bool door_open = false;
  bool sig_220_out = false;
  bool sig_220_in = false;
  bool sig_in_locked = false;
  const int wait_delay= 100;

  
  /*unsigned char c = 0;*/
  bool uart_com = false;
  bool arg = false;
  uint8_t bus_argc = 0;
  int uart_command = 0;
  int uart_arg = 0;

  bool sig_5v = false;
  bool sig_eis = false;
  bool sig_radio = false;

  // lock stuff 
  int lock_cnt = 1;
  int lock_action = 0; // do nothing

  /* enable pull ups */
  IN_PORT_DOOR &= ~( 1 << IN_PIN_DOOR );
  IN_PORT_LOCKED &= ~( 1 << IN_PIN_LOCKED );

  //DDRD = DDRD | 0b11111000; //PORTD (RX on PD0), IRQ on PD2
	while(1)
	{

    ++cnt;

    if (cnt%500==0) {
      set_new_led_color();
    }

    // check if any of the doors are open
    if (!bit_is_set(PINC, IN_PIN_DOOR)) {
      door_open = true;
    } else {
      door_open = false;
    }

    // check if any of the doors are open
    if (!bit_is_set(PINC, IN_PIN_LOCKED)) {
      sig_in_locked = true;
    } else {
      sig_in_locked = false;
    }

    // check if 220V are available
    uint16_t analog_val_220 = adc_read(IN_220_SENSE);
    if (analog_val_220 < 200) {
      sig_220_in = true;
    } else {
      sig_220_in = false;
    }

    // read cap touch via i2c
		touchstatus = mpr121Read(0x00);	// Read touch status
		touchstatus = touchstatus << 2;	// Shift two bits over
    if (touch_cnt > 0)
      --touch_cnt;
    
    ++step;
    ++alarm_cnt;
    ++lock_cnt;
    
    if (step > 100000) 
      step = 0;
    
    bool touch_change = false;
    if (touch_cnt == 0) {
      for (int i = 2; i < 8; ++i) {
        if (touchstatus & (1<<i)) {
          touch_cnt = wait_delay;
          touch_change = true;
          if (state & (1<<i))
            state &= ~(1<<i);
          else 
            state |= (1<<i);
        }
      }
    }
    
    if (touch_change) {
      //enables light
      if (state & TOUCH_LIGHT) touched_light = true;
      else touched_light = false;
      if (state & TOUCH_EIS) touched_eis = true;
      else touched_eis = false;
      if (state & TOUCH_5V) touched_5v = true;
      else touched_5v = false;
      if (state & TOUCH_MISC) touched_misc = true;
      else touched_misc = false;
      if (state & TOUCH_LOCK) touched_lock = true;
      else touched_lock = false;
      if (state & TOUCH_RADIO) touched_radio = true;
      else touched_radio = false;
    }
 
    if (touched_5v) {
      sig_5v = true;
    } else {
      sig_5v = false;
    }

    if (touched_eis) {
      sig_eis = true;
    } else {
      sig_eis = false;
    }

    if (touched_radio) {
      sig_radio = true;
    } else {
      sig_radio = false;
    }

    if (touched_lock) {
      // we want to lock or unlock the car. 
      // this is based on the state of the car.
      if (sig_in_locked) // car is locked, so we want to unlock it
        lock_action = 1;
      else 
        lock_action = 2;
      lock_cnt = 0;
      state &= ~(TOUCH_LOCK);
      touched_lock = false;
    }

    sig_220_out = sig_220_in;

    if (alarm != ALARM) {
      if (door_open || touched_light) {
        if (white_fade < 128) {
          ++white_fade;
          _delay_ms(5);
        }
        led_color[LED_COLOR_WHITE] = white_fade;
      } else if (white_fade > 0) {
        --white_fade;
        _delay_ms(5);
        led_color[LED_COLOR_WHITE] = white_fade;
      } else {
        white_fade = 0;
        led_color[LED_COLOR_WHITE] = 0;
      }
    }

    set_output(&OUT_5V_PORT, OUT_5V_PIN, sig_5v);
    set_output(&OUT_EIS_PORT, OUT_EIS_PIN, sig_eis);
    set_output(&OUT_RADIO_PORT, OUT_RADIO_PIN, sig_radio);
    set_output(&OUT_220_PORT, OUT_220_PIN, sig_220_out);

    if (lock_cnt == 0) {
      if (lock_action == 1)
        set_output(&OUT_UNLOCK_PORT, OUT_UNLOCK_PIN, 1);
      else if (lock_action == 2)
        set_output(&OUT_LOCK_PORT, OUT_LOCK_PIN, 1);

    } else if (lock_cnt == MAX_LOCK_CNT) { 
      set_output(&OUT_LOCK_PORT, OUT_LOCK_PIN, 0);
      set_output(&OUT_UNLOCK_PORT, OUT_UNLOCK_PIN, 0);
    } else if (lock_cnt > MAX_LOCK_CNT) {
      lock_cnt = 1;
    }

    led_color[LED_COLOR_GREEN] = 0;
    led_color[LED_COLOR_BLUE] = 0;
    led_color[LED_COLOR_RED] = 0;

    // calculate new LED state 
    int new_state = state;
    if (door_open || touched_light)
      new_state |= (TOUCH_LIGHT);
    else 
      new_state &= ~(TOUCH_LIGHT);
    if (sig_in_locked)
      new_state |= (TOUCH_LOCK);
    else
      new_state &= ~(TOUCH_LOCK);

    if (new_state != old_new_state) {
      mpr121Write(GPIO_CLEAR, 0xFF);	// Clear all LEDs
      mpr121Write(GPIO_SET, new_state);	// Set LED 
      old_new_state = new_state;
    }
    
    continue;
    
    c = uart_getc();
    if (!(c&UART_NO_DATA)) {
      //uart_putc( (unsigned char)c );
      if (c == 60) { // start communication with '<'
        uart_com = true;
        arg = false;
        uart_command = 0;
        uart_arg = 0;
  
      } else if (!uart_com) {
        // foo
      
      } else if (c == 62) { // stop communication with '>'
        uart_com = false;
        // run my commands!
        if (uart_command == 48) { // 0
          if (uart_arg == 48) { // off 
            alarm_requested = false; 
            alarm = ALARM_OFF;
            state &= ~(1<<3);
            red_on = false;
          } else if (uart_arg == 49) { // on     
            alarm_requested = true;
            alarm_cnt = 0;
          }
        
        } else if (uart_command == 49) { // 0
          if (uart_arg == 49) { // alarm triggered
            alarm_triggered = true;
          } else if (uart_arg == 48) {
            alarm_triggered = false;
          }
        }

      } else {
        if (!arg) {
          uart_command = c;
          arg = true;
        } else {
          uart_arg = c;
        }
      }

    }
  
    // request alarm over UART
    if (alarm_requested) {
      red_on = true;
      state |= (1<<3);
    }

    if (alarm == ALARM_OFF) {
      alarm_triggered = false;
      led_color[LED_COLOR_RED] = 0;
      if (red_on) {
        alarm = ALARM_WAIT;
        alarm_cnt = 0;
      }
    } else if (alarm == ALARM_WAIT) {
      if (!red_on) 
        alarm = ALARM_OFF;
      if (alarm_cnt > 2*128 && alarm_cnt < 4*128) {
        led_color[LED_COLOR_RED] = 0;
      } else if (alarm_cnt > 5*128) {
        alarm = ALARM_FOCUS;
        //USART_putstring("<0|1>");
        led_color[LED_COLOR_RED] = 0;
      } else {
        led_color[LED_COLOR_RED] = 2*(alarm_cnt % 128);
      }
    } else if (alarm == ALARM_FOCUS) {
      if (door_open || alarm_triggered) {
        alarm_cnt = 0;
        alarm = ALARM;
      }
      if (!red_on) {
        alarm = ALARM_OFF;
        //USART_putstring("<0|0>");
      }
    } else if (ALARM == alarm) {
      if (!red_on) {
        alarm = ALARM_OFF;
        //USART_putstring("<0|0>");
      }
      if (!alarm_triggered && !door_open && (alarm_cnt > 1000)) {
        alarm = ALARM_FOCUS;
        alarm_state = false;
        alarm_cnt = 1;
        led_color[LED_COLOR_RED] = 0;
      }
      led_color[LED_COLOR_WHITE] = 0;
      if (alarm_cnt % 100 == 0)
        alarm_state = !alarm_state;
      if (alarm_state)
        led_color[LED_COLOR_RED] = 128;
      else 
        led_color[LED_COLOR_RED] = 0;

    }
	}
}




/*
 * ALL THOSE functions
 */
uint16_t adc_read(uint8_t adcx) {
  ADMUX &= 0xF0;                    //Clear the older channel that was read
  ADMUX |= adcx;                //Defines the new ADC channel to be read
  ADCSRA |= (1<<ADSC);                //Starts a new conversion
  while(ADCSRA & (1<<ADSC));            //Wait until the conversion is done
  return ADCW;  
}

ISR (TIMER2_COMPA_vect) {
    static uint8_t pwm_cnt=0;
    OCR1A += (uint16_t)T_PWM;
        
    //set_output(&LED_PORT_WHITE, LED_PIN_WHITE, (led_color[LED_COLOR_WHITE] > pwm_cnt));
    if (led_color[LED_COLOR_WHITE] > pwm_cnt)
      LED_PORT_WHITE |= (_BV(LED_PIN_WHITE));
    else
      LED_PORT_WHITE &= ~(_BV(LED_PIN_WHITE));

    //set_output(&LED_PORT_BLUE, LED_PIN_BLUE, (led_color[LED_COLOR_BLUE] > pwm_cnt));
    if (led_color[LED_COLOR_BLUE] > pwm_cnt)
      LED_PORT_BLUE |= (_BV(LED_PIN_BLUE));
    else
      LED_PORT_BLUE &= ~(_BV(LED_PIN_BLUE));
    
    //set_output(&LED_PORT_RED, LED_PIN_RED, (led_color[LED_COLOR_RED] > pwm_cnt));
    if (led_color[LED_COLOR_RED] > pwm_cnt)
      LED_PORT_RED |= (_BV(LED_PIN_RED));
    else
      LED_PORT_RED &= ~(_BV(LED_PIN_RED));
    
    //set_output(&LED_PORT_GREEN, LED_PIN_GREEN, (led_color[LED_COLOR_GREEN] > pwm_cnt));
    if (led_color[LED_COLOR_GREEN] > pwm_cnt)
      LED_PORT_GREEN |= (_BV(LED_PIN_GREEN));
    else
      LED_PORT_GREEN &= ~(_BV(LED_PIN_GREEN));
    
    //set_output(&LED_PORT_TOUCH, LED_PIN_TOUCH, (led_color[LED_COLOR_TOUCH] > pwm_cnt));
    if (led_color[LED_COLOR_TOUCH] > pwm_cnt)
      LED_PORT_TOUCH |= (_BV(LED_PIN_TOUCH));
    else
      LED_PORT_TOUCH &= ~(_BV(LED_PIN_TOUCH));

    //PWM_PORT = tmp;                         // PWMs aktualisieren
    if (pwm_cnt==(uint8_t)(PWM_STEPS-1))
        pwm_cnt=0;
    else
        pwm_cnt++;
}

uint8_t set_new_led_color() {
  uint16_t val = adc_read(IN_LIGHT_SENSE); // / 4;     // read the input pin 

#if 0
  USART_putstring("raw value: ");
  char str[16];
  itoa(val, str, 10);
  USART_putstring(str);
  USART_putstring("\n");
#endif 

  int t = val - 130;
  float rat = t / 500.;
  float full = rat * 255.f;
  uint8_t ret = 0;

  if (full < 10.) 
    ret = 10;
  else if (full > 255.)
    ret = 255;
  else 
    ret = (uint8_t)full;
 
#if 0
  USART_putstring("set to value: ");
  char str2[16];
  itoa(ret, str2, 10);
  USART_putstring(str2);
  USART_putstring("\n");
#endif 
  led_color[LED_COLOR_TOUCH] = ret;
}

void set_output(volatile uint8_t *port, uint8_t pin, bool value) {
  if (value)
    *port |= (_BV(pin));
  else
    *port &= ~(_BV(pin));
}
void USART_init(void){
 UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8);
 UBRR0L = (uint8_t)(BAUD_PRESCALLER);
 UCSR0B = (1<<RXEN0)|(1<<TXEN0);
 UCSR0C = (3<<UCSZ00);
}
 
unsigned char USART_receive(void){
 while(!(UCSR0A & (1<<RXC0)));
 return UDR0;
}
 
void USART_send( unsigned char data){
 while(!(UCSR0A & (1<<UDRE0)));
 UDR0 = data;
}
 
void USART_putstring(char* StringPtr){
while(*StringPtr != 0x00){
 USART_send(*StringPtr);
 StringPtr++;}
}

char mpr121Read(unsigned char address)
{
	char data;
	
	i2cSendStart();
	i2cWaitForComplete();
	
	i2cSendByte(MPR121_W);	// write 0xB4
	i2cWaitForComplete();
	
	i2cSendByte(address);	// write register address
	i2cWaitForComplete();
	
	i2cSendStart();
	
	i2cSendByte(MPR121_R);	// write 0xB5
	i2cWaitForComplete();
	i2cReceiveByte(TRUE);
	i2cWaitForComplete();
	
	data = i2cGetReceivedByte();	// Get MSB result
	i2cWaitForComplete();
	i2cSendStop();
	
	cbi(TWCR, TWEN);	// Disable TWI
	sbi(TWCR, TWEN);	// Enable TWI
	
	return data;
}

void mpr121Write(unsigned char address, unsigned char data)
{
	i2cSendStart();
	i2cWaitForComplete();
	
	i2cSendByte(MPR121_W);	// write 0xB4
	i2cWaitForComplete();
	
	i2cSendByte(address);	// write register address
	i2cWaitForComplete();
	
	i2cSendByte(data);
	i2cWaitForComplete();
	
	i2cSendStop();
}

// MPR121 Quick Config
// This will configure all registers as described in AN3944
// Input: none
// Output: none
void mpr121QuickConfig(void)
{
	// Section A
	// This group controls filtering when data is > baseline.
	mpr121Write(MHD_R, 0x01);
	mpr121Write(NHD_R, 0x01);
	mpr121Write(NCL_R, 0x00);
	mpr121Write(FDL_R, 0x00);
	
	// Section B
	// This group controls filtering when data is < baseline.
	mpr121Write(MHD_F, 0x01);
	mpr121Write(NHD_F, 0x01);
	mpr121Write(NCL_F, 0xFF);
	mpr121Write(FDL_F, 0x02);
	
	// Section C
	// This group sets touch and release thresholds for each electrode
	mpr121Write(ELE0_T, TOU_THRESH);
	mpr121Write(ELE0_R, REL_THRESH);
	mpr121Write(ELE1_T, TOU_THRESH);
	mpr121Write(ELE1_R, REL_THRESH);
	mpr121Write(ELE2_T, TOU_THRESH);
	mpr121Write(ELE2_R, REL_THRESH);
	mpr121Write(ELE3_T, TOU_THRESH);
	mpr121Write(ELE3_R, REL_THRESH);
	mpr121Write(ELE4_T, TOU_THRESH);
	mpr121Write(ELE4_R, REL_THRESH);
	mpr121Write(ELE5_T, TOU_THRESH);
	mpr121Write(ELE5_R, REL_THRESH);
	
  /*mpr121Write(ELE6_T, TOU_THRESH);
	mpr121Write(ELE6_R, REL_THRESH);
	mpr121Write(ELE7_T, TOU_THRESH);
	mpr121Write(ELE7_R, REL_THRESH);
	mpr121Write(ELE8_T, TOU_THRESH);
	mpr121Write(ELE8_R, REL_THRESH);
	mpr121Write(ELE9_T, TOU_THRESH);
	mpr121Write(ELE9_R, REL_THRESH);
	mpr121Write(ELE10_T, TOU_THRESH);
	mpr121Write(ELE10_R, REL_THRESH);
	mpr121Write(ELE11_T, TOU_THRESH);
	mpr121Write(ELE11_R, REL_THRESH);*/
	
	// Section D
	// Set the Filter Configuration
	// Set ESI2
	mpr121Write(FIL_CFG, 0x04);
	
	// Section E
	// Electrode Configuration
	// Enable 6 Electrodes and set to run mode
	// Set ELE_CFG to 0x00 to return to standby mode
	// mpr121Write(ELE_CFG, 0x0C);	// Enables all 12 Electrodes
	mpr121Write(ELE_CFG, 0x06);		// Enable first 6 electrodes
	
	// Section F
	// Enable Auto Config and auto Reconfig
	/*mpr121Write(ATO_CFG0, 0x0B);
	mpr121Write(ATO_CFGU, 0xC9);	// USL = (Vdd-0.7)/vdd*256 = 0xC9 @3.3V
	mpr121Write(ATO_CFGL, 0x82);	// LSL = 0.65*USL = 0x82 @3.3V
	mpr121Write(ATO_CFGT, 0xB5);*/	// Target = 0.9*USL = 0xB5 @3.3V
}

int checkInterrupt(void)
{
	if ((PIND & (1<<2)) == 0)
		return 0;
	else
		return 1;
}

/*********************
 ****Initialize****
 *********************/
 
void ioinit (void)
{
    //1 = output, 0 = input
	DDRB = 0b00111111; //PORTB4, B5 output
  DDRC = 0b00110001; //PORTC4 (SDA), PORTC5 (SCL), PORTC all others are inputs
  DDRD = 0b11111010; //PORTD (RX on PD0), IRQ on PD2
	
  PORTC = 0b00110000; //pullups on the I2C bus
  
  //DDRC = 0b00010011; //PORTC4 (SDA), PORTC5 (SCL), PORTC all others are inputs
}
