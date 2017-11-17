#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "softuart.h"
#include <util/delay.h>

int main(void)
{
	char c;

	softuart_init();
	softuart_turn_rx_on(); /* redundant - on by default */
	
	sei();

	for (;;) {
		if ( softuart_kbhit() ) {
			c = softuart_getchar();
			softuart_puts_P("Hello\n");
		}
		
	}
	
	return 0; /* never reached */
}
