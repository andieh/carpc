#include <avr/pgmspace.h>
#include <stdbool.h>

void setRGB(uint8_t r, uint8_t g, uint8_t b, bool force);
uint16_t animate(uint16_t cnt, uint8_t start);
