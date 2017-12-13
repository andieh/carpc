#include <avr/pgmspace.h>

uint8_t debounce(volatile uint8_t *port, uint8_t pin);
uint16_t adc_read(uint8_t channel);
