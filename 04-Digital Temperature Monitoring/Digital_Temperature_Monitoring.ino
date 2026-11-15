//Project 04: Digital Temperature Monitoring
// Arduino Uno + TMP36 Sensor

const int sensorPin = A0;

float voltage;
float temperature;

void setup() {
  Serial.begin(9600);
}

void loop() {

  // Read analog value from TMP36
  int sensorValue = analogRead(sensorPin);

  // Convert analog reading into voltage
  voltage = sensorValue * (5.0 / 1023.0);

  // Convert voltage into temperature in Celsius
  temperature = (voltage - 0.5) * 100.0;

  // Display temperature
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");

  delay(1000);
}
