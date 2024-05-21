const double scale_factor = 0.185; // 20A 100 mV/
float current_sensor_voltage = 0.00;
double current = 0;
float current_sensor_value = 0;
int cycle_count = 10000;
float elapsed_time = 0;

// voltage divider
float battery_sensor_value = 0;
float battery_sensor_voltage = 0;
float battery_voltage = 0;
const float R1 = 47000.0;
const float R2 = 10000.0;


// Calibrate zero point for current sensor (assuming no current flow at start)
const int sensorPin = A0;// Current Sensor Pin
const int Calibration_sample = 1000; 
double zero_point = 0.0;


void setup() {
  Serial.begin(9600);
  delay(250); // wait for the OLED to power up
 zero_point = calibrateZeroPoint();// Put in main por favor n1gga - From Habibi
  analogReference(DEFAULT); // No need for this line if Default
}

void loop() {
  long current_milli = millis(); // need an accurate time elapsed value so energy can be calculated

  // Reset values for current sensor voltage and battery voltage before the main loop
  current_sensor_voltage = 0;
  battery_voltage = 0;

  // loop several times for better accuracy
  for (int i = 0; i < cycle_count; i++) {
    // Current Value
    current_sensor_value = analogRead(A0);
    current_sensor_voltage += (current_sensor_value * 5.0 / 1024.0);

    // Voltage
    battery_sensor_value = analogRead(A1);
    battery_sensor_voltage = (battery_sensor_value * 5.0) / 1024.0;
    battery_voltage += battery_sensor_voltage / (R2 / (R1 + R2));

    delay(1);
  }

  delay(1000); // wait full 1 second

  // Calculate current sensor voltage by dividing by the cycle count
  current_sensor_voltage /= cycle_count;
  battery_voltage /= cycle_count;

  // Convert current sensor voltage output into Current value using Scale Factor
  current = (current_sensor_voltage - zero_point) / scale_factor;
  power = battery_voltage * current;

  // Measure the time since the loop started
  long nu = millis();
  elapsed_time = (nu - current_milli) / 1000.0; // outputs in seconds


  // Show results
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

double calibrateZeroPoint() { // Function Definiation 
  double sensor_voltage = 0.0;
  int sensorValue;

  for (int i = 0; i < Calibration_sample; i++) {
    sensorValue = analogRead(A0);
    sensor_voltage += (sensorValue * 5.0 / 1024.0);
    delay(1);
  }

  // Calculate the avg voltage
  return sensor_voltage / Calibration_sample;
}
