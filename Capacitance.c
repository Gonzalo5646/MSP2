#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "usart.h"

// Define pin
#define CHARGE_PIN PD5       
#define DISCHARGE_PIN PD3    
#define ANALOG_PIN PC0       

// Constants
#define RESISTOR_VALUE 10000.0F
#define ADC_THRESHOLD 648    


volatile uint32_t ms_counter = 0; // Millisecond counter

// Function prototypes
void initADC();
uint16_t readADC(uint8_t channel);
void initTimer1(void);
uint32_t millis(void);

int main(void) {
    DDRD |= (1 << CHARGE_PIN);   // Set charge pin as output
    DDRD &= ~(1 << DISCHARGE_PIN); // Set discharge pin as input

    uart_init();
    initADC();
    initTimer1();
    sei(); 

    uint16_t elapsedTime;
    float microFarads, nanoFarads;

    while (1) {
        PORTD |= (1 << CHARGE_PIN);  // Set charge pin high
        uint32_t startTime = millis();

        // Wait until the analog value reaches the threshold
        while (readADC(ANALOG_PIN) < ADC_THRESHOLD);

        elapsedTime = millis() - startTime;
        microFarads = ((float)elapsedTime / RESISTOR_VALUE) * 1000.0;

        printf(elapsedTime);
        printf(" mS    ");

        if (microFarads > 1) {
            printf((long)microFarads);
            printf(" microFarads\n");
        } else {
            nanoFarads = microFarads * 1000.0;
            printf((long)nanoFarads);
            printf(" nanoFarads\n");
        }

        PORTD &= ~(1 << CHARGE_PIN); // Set charge pin low
        DDRD |= (1 << DISCHARGE_PIN); // Set discharge pin as output
        PORTD &= ~(1 << DISCHARGE_PIN); // Set discharge pin low

        // Wait until the capacitor is fully discharged
        while (readADC(ANALOG_PIN) > 0);

        DDRD &= ~(1 << DISCHARGE_PIN); // Set discharge pin as input
        _delay_ms(500); // Delay to allow things to settle
    }
}

void initADC() {
    ADMUX = (1 << REFS0); // AVcc with external capacitor at AREF pin
    ADCSRA = (1 << ADEN) | (1 << ADPS1) | (1 << ADPS0); // Enable ADC and set prescaler to 8
}

uint16_t readADC(uint8_t channel) {
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
    ADCSRA |= (1 << ADSC); // Start conversion
    while (ADCSRA & (1 << ADSC)); // Wait until conversion is complete
    return ADC;
}


void initTimer1(void) {
    TCCR1B |= (1 << WGM12); // Configure timer 1 for CTC mode
    TIMSK1 |= (1 << OCIE1A); // Enable CTC interrupt
    OCR1A = 249; // (16MHz / 64 prescaler) / 1000 - 1 = 249
    TCCR1B |= (1 << CS11) | (1 << CS10); // Start timer at Fcpu/64
}

ISR(TIMER1_COMPA_vect) {
    ms_counter++; // Increment ms counter
}

uint32_t millis(void) {
    uint32_t millis_copy;
    cli(); 
    millis_copy = ms_counter; 
    sei(); 
    return millis_copy;
}




















