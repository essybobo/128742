#include <DHT.h>

#define DHTPIN D5  // Pin where the DHT11 sensor is connected
#define DHTTYPE DHT11  // DHT11 sensor type

#define LED_PIN D4  // Pin where the LED is connected
#define pump_pin D6

// Create an instance of the DHT sensor
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  
  // Set the LED pin as an output
  pinMode(LED_PIN, OUTPUT);
  pinMode(pump_pin,OUTPUT);

  // Initialize the DHT sensor
  dht.begin();
}

void loop() {
  // Read temperature and humidity from DHT sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Check if any reads failed
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print temperature and humidity to the serial monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Check temperature value and control LED
  if (temperature > 20.0) {
    digitalWrite(LED_PIN, HIGH);  // Turn on LED if temperature is above 20.0 degrees Celsius
     digitalWrite(pump_pin, HIGH);
     Serial.print("PUMP ON");
  } else {
    digitalWrite(LED_PIN, LOW);   // Turn off LED otherwise
    digitalWrite(pump_pin, LOW);
  }

  // Delay before reading again
  delay(5000);
}
