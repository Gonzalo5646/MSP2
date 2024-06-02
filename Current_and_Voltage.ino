const double scale_factor = 0.1; // 20A 100 mV/A model 
double current_sensor_voltage = 0.00;
double current = 0;
double zero_point = 2.5;
int current_sensor_value = 0;  
int cycle_count = 1000;
float elapsed_time = 0;

// Voltage divider - 
int battery_sensor_value = 0;  // Change from float to int 
double battery_sensor_voltage = 0;  // Change from float to double
double battery_voltage = 0;  // Change from float to double
const float R1 = 47000.0;
const float R2 = 10000.0; // 

void setup() { // MAGIC TIME 
  Serial.begin(9600);
  delay(250);
}

void loop() {
  long current_milli = millis(); 

  // Reset values for current sensor voltage and battery voltage before the loop
  current_sensor_voltage = 0;
  battery_voltage = 0;

  // Loop several times for better accuracy 
  for (int i = 0; i < cycle_count; i++) {
    // Current Value
    current_sensor_value = analogRead(A0);
    current_sensor_voltage += (current_sensor_value * 5.0 / 1024.0);

    battery_sensor_value = analogRead(A1);
    battery_sensor_voltage = (battery_sensor_value * 5.0) / 1024.0;
    battery_voltage += battery_sensor_voltage / (R2 / (R1 + R2));

    delay(1);
  }

  delay(1000); // Wait 

  // Calculate current sensor voltage by dividing by the cycle count. 
  current_sensor_voltage /= cycle_count;
  battery_voltage /= cycle_count;

  // Convert current sensor voltage output into the I Reading using the sensitivity 
  current = (current_sensor_voltage - zero_point) / scale_factor;
  power = battery_voltage * current;

  // Measure time of the loop
  long nu = millis();
  elapsed_time = (nu - current_milli) / 1000.0; // Outputs in seconds


  Serial.print("Voltage:   ");
  Serial.print(battery_voltage);
  Serial.println(" V");

  Serial.print("Current:   ");
  Serial.print(current);
  Serial.println(" A");

  Serial.print("Current:   ");
  Serial.print(current * 1000);
  Serial.println(" mA");

}
