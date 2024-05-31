#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "usart.h"
#define BAUDRATE 9600
#define UBRRVAL ((F_CPU/(BAUDRATE*16UL))-1)

void ADC_init(void) {
    // AVcc reference, ADC0 (PC0) single ended input
    ADMUX = (1 << REFS0);        
    // Enable ADC, prescaler = 128
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); 
}

uint16_t ADC_read(void) {
    // Start conversion
    ADCSRA |= (1 << ADSC);
    // Wait for conversion to complete
    while (ADCSRA & (1 << ADSC));
    // Return ADC value
    return ADC;
}

int main(void) {
    // Set baud rate
    uart_init();
    ADC_init();

    float Vin = 5.0;
    float Vout, R1 = 10000; // Changed R1 to 10k Ohm
    float R2, bufferf;

    while(1) {
        uint16_t raw = ADC_read();
        if(raw) {
            bufferf = raw * Vin;
            Vout = (bufferf) / 1024.0;
            bufferf = (Vin / Vout) - 1;
            R2 = R1 * bufferf;

            
            printf(Vout, 5, 2, buffer);
            printf("Vout: ");
            printf(buffer);
            printf("\n");

            printf(R2, 5, 2, buffer);
            printf("R2: ");
            printf(buffer);
            printf("\n");

            _delay_ms(1000);
        }
    }

    return 0;
}































