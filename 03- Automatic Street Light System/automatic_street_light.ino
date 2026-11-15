// Automatic Street Light System

const int LDR_PIN = A0;
const int LED_PIN = 12;

int lightValue = 0;
int threshold = 500;

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  
  Serial.begin(9600);
}

void loop()
{
  // Red Light Intensity
  lightValue = analogRead(LDR_PIN);
  
  // Display sensor value
  Serial.print("Light Value: ");
  Serial.println(lightValue);
  
  // Automatic street light control
  if (lightValue < threshold) {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("DARK - LED ON");
  }
  else {
    digitalWrite(LED_PIN, LOW);
    Serial.println("BRIGHT - LED OFF ");
  }
  delay(500);
}
