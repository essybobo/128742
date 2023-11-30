#include <DHT.h>
#include <Ticker.h>

#define DHTPIN D5  // Pin where the DHT22 sensor is connected
#define DHTTYPE DHT11  // DHT22 sensor type
#define LED_PIN D4  // Pin where the LED is connected

// Create an instance of the DHT sensor
DHT dht(DHTPIN, DHTTYPE);

const int ledOnDuration = 10; // 10 seconds
const int ledOffDuration = 2 * 60 * 1000; // 2 minutes in milliseconds
Ticker ledTicker;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  dht.begin();

  // Set up the initial state
  digitalWrite(LED_PIN, LOW);

  // Start the timer
  ledTicker.attach(ledOffDuration, turnOffLED); // Attach the callback function
}

void loop() {
  // Read temperature and humidity from DHT sensor
  float temperature = dht.readTemperature();
  if (isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  // Check temperature value and control LED
  if (temperature > 26.0) {
    if (digitalRead(LED_PIN) == LOW) {
      digitalWrite(LED_PIN, HIGH);
      ledTicker.once(ledOnDuration, turnOffLED); // Schedule turnOffLED after ledOnDuration
    }
  }
}

void turnOffLED() {
  // Turn off the LED immediately
  digitalWrite(LED_PIN, LOW);
}
