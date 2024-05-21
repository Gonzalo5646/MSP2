#include <Wire.h>

double pulse, frequency, capacitance, inductance;
const double resistorValue = 150; // Value of the resistor in ohms


void setup() {
  Serial.begin(115200);
  pinMode(11, INPUT);
  pinMode(13, OUTPUT);
  delay(200);
}

void loop() {
  digitalWrite(13, HIGH);
  delay(5); // Give some time to charge inductor
  digitalWrite(13, LOW);
  delayMicroseconds(100); // Make sure the resonance is measured

  pulse = pulseIn(11, HIGH, 10000); // Increase timeout to 10 ms

  if (pulse > 0.1) { // If a timeout did not occur and it took a reading
    capacitance = 1.0E-6; // Value of the capacitor in farads (adjust as necessary)
    frequency = 1.0E6 / (2 * pulse);
    
    // Calculate inductance
    inductance = (1.0 / (capacitance * frequency * frequency * 4 * PI * PI)) * 1E6; // Convert to uH

    // Display on serial monitor
    Serial.print("High for uS:");
    Serial.print(pulse);
    Serial.print("\tFrequency Hz:");
    Serial.print(frequency);
    Serial.print("\tInductance uH:");
    Serial.println(inductance);
    Serial.print("\tinductance mH:");
    Serial.println(inductance/1000);
    delay(1000); 
  }
}

