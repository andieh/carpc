#include "buttons.h"
#include <util/delay.h>

/* Einfache Funktion zum Entprellen eines Tasters */
uint8_t debounce(volatile uint8_t *port, uint8_t pin) {
    if ( (*port & (1 << pin)) ) {
        //softuart_puts_P("entprellen!!\n");
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
