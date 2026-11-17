#include <Wire.h>
#include <SPI.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <DHT.h>
#include <RTClib.h>

// MAX7219 LED Matrix Configuration
#define HARDWARE_TYPE MD_MAX72XX::PAROLA_HW
#define MAX_DEVICES 4
#define CS_PIN 10

MD_Parola display = MD_Parola(
  HARDWARE_TYPE, CS_PIN, MAX_DEVICES
);

// DHT22 Configuration
#define DHT_PIN 2
#define DHT_TYPE DHT22

DHT dht(DHT_PIN, DHT_TYPE);

// RTC Configuration
RTC_DS1307 rtc;

// Variables
char message[50];
bool showClock = true;

void setup() {

  Serial.begin(9600);

  // Initialize LED Matrix
  display.begin();
  display.setIntensity(5);
  display.displayClear();

  // Initialize DHT22
  dht.begin();

  // Initialize RTC
  if (!rtc.begin()) {
    Serial.println("RTC not found!");
    while (1);
  }

  if (!rtc.isrunning()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  Serial.println("Weather Station Started!");

  prepareMessage();
}

// Prepare clock or weather message
void prepareMessage() {

  if (showClock) {

    // Read time from RTC
    DateTime now = rtc.now();

    snprintf(message, sizeof(message),
             "TIME %02d:%02d",
             now.hour(), now.minute());

  } else {

    // Read temperature and humidity
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (isnan(temperature) || isnan(humidity)) {

      snprintf(message, sizeof(message),
               "SENSOR ERROR");

    } else {

      // Convert floating values to integers
      int tempValue = (int)temperature;
      int humValue = (int)humidity;

      snprintf(message, sizeof(message),
               "TEMP %dC HUM %d%%",
               tempValue, humValue);

      // Serial Monitor output
      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.println(" C");

      Serial.print("Humidity: ");
      Serial.print(humidity);
      Serial.println(" %");
    }
  }

  // Print matrix message in Serial Monitor
  Serial.print("Matrix message: ");
  Serial.println(message);

  // Switch between clock and weather
  showClock = !showClock;

  // Display scrolling message
  display.displayText(
    message,
    PA_LEFT,
    100,
    500,
    PA_SCROLL_LEFT,
    PA_SCROLL_LEFT
  );
}

// Main loop
void loop() {

  // Restart scrolling after each message
  if (display.displayAnimate()) {
    prepareMessage();
  }

}