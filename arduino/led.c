#include "led.h"
#include <util/delay.h>

#define LED_DIM_SPEED 5

uint8_t calcValue(uint8_t color, uint8_t current) {
    //set led dimm speed
    uint8_t step = LED_DIM_SPEED;

    uint8_t newValue = color;
    if (newValue <= (current-step))
        newValue = current - step;
    else if (newValue >= (current+step))
        newValue = current + step;
    
    if (newValue > 0xFF)
        newValue = 0xFF;
    if (newValue < 0x00)
        newValue = 0x00;

    return newValue;
}

void setRGB(uint8_t r, uint8_t g, uint8_t b, bool force) {

    uint8_t current_r = OCR1A;
    uint8_t current_g = OCR2A;
    uint8_t current_b = OCR2B;

    uint8_t new_r = r;
    uint8_t new_g = g;
    uint8_t new_b = b;
    if (!force) 
        new_r = calcValue(r, current_r);
        new_g = calcValue(g, current_g);
        new_b = calcValue(b, current_b);

    bool update_r = (current_r != new_r);
    bool update_g = (current_g != new_g);
    bool update_b = (current_b != new_b);
    if (!(update_r || update_g || update_b))
        return;

    // set value
    OCR1A = new_r;
    OCR2A = new_g;
    OCR2B = new_b;

    // wait some time, otherwise this is way too fast!
    _delay_ms(10);
        
}

/*void setR(uint8_t color, bool force) {
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
        // enable PWM
		TCCR1A |= _BV(COM1A1) | _BV(WGM10);
    	//TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
    }

    uint8_t newValue = color;
    // force to set value
    if (!force) {
		//softuart_puts_P("interpolate\n");
        if (newValue <= (current-step)) {
            newValue = current - step;
            _delay_ms(10);
        }
        else if (newValue >= (current+step)) {
            newValue = current + step;
            _delay_ms(10);
        }
        else
            newValue = color;
    }

    if (newValue > 0xFF) 
        newValue = 0XFF;
    if (newValue <= 0x00) {
        // disable PWM
        OCR1A = 0x00;
        TCCR1A &= (0<<COM1A1);
        //TCCR2A &= (0<<COM2A1) & (0<<COM2B1);
        return;
    }
    OCR1A = newValue;
}

*/
