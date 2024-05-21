#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#define BAUDRATE 9600
#define UBRRVAL ((F_CPU/(BAUDRATE*16UL))-1)

void USART_init(void) {
    UBRR0H = (uint8_t)(UBRRVAL >> 8);
    UBRR0L = (uint8_t)UBRRVAL;
    UCSR0B = (1<<TXEN0)|(1<<RXEN0);
    UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
}

void USART_tx_char(unsigned char data) {
    while (!(UCSR0A & (1<<UDRE0)));
    UDR0 = data;
}

void USART_tx_string(const char *data) {
    while (*data) {
        USART_tx_char(*data++);
    }
}

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
    USART_init();
    ADC_init();

    char buffer[20];
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

            // Convert float values to string
            dtostrf(Vout, 5, 2, buffer);
            USART_tx_string("Vout: ");
            USART_tx_string(buffer);
            USART_tx_string("\n");

            dtostrf(R2, 5, 2, buffer);
            USART_tx_string("R2: ");
            USART_tx_string(buffer);
            USART_tx_string("\n");

            _delay_ms(1000);
        }
    }

    return 0;
}































