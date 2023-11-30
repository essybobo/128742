
//c,Esther Wambui 2023

#include <ESP8266WiFi.h>
#include "AdafruitIO_WiFi.h"
#include <DHT.h>

// DHT sensor pin
const int dhtPin = D5;
const int LED = D4;           //for the LEd
const int PUMP_PIN = D6;      // for the relay to pump
unsigned long PUMP_ON_DURATION=6000;   //time to power the pump for
unsigned long delayTime=1000;          //to delay sending to serial monitor
unsigned long previousTime = 0;        // to control sending to serial monitor 
unsigned long lastChangeTime=0;        //to control pump on duration
float temperature;
float humidity;

// Initialize DHT sensor
DHT dht(dhtPin, DHT11);



#define WIFI_SSID       "The Mbugua's_Ext"
#define WIFI_PASS       "GMbugua@123"

#define IO_USERNAME    "essybobo"
#define IO_KEY         "aio_hMEP08gEq7T8EAsYzBkifqgo8U0O"

// Connect to Wi-Fi and Adafruit IO handel
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

// Create a feed object that allows us to send data to
AdafruitIO_Feed * TEMPERATURE = io.feed("temperature");
AdafruitIO_Feed * HUMIDITY = io.feed("humidity");


void setup()
{
  // Enable the serial port so we can see updates
  Serial.begin(115200);
  pinMode(LED, OUTPUT);  // declare the led as an output
  pinMode(PUMP_PIN, OUTPUT);  ///declare pump pin as an output

  // Connect to Adafruit IO
  io.connect();

  // wait for a connection
  while (io.status() < AIO_CONNECTED)
  {
    delay(1000);
    Serial.println("*****");
    Serial.println("***Connecting to Wi-Fid***");

  }
  Serial.println("Connected to WiFi");
  // Initialize DHT sensor
  dht.begin();

}

void loop()
{

  sensor();

}

void sensor() {

  io.run();

  // Read temperature and humidity from DHT sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Check if any reads failed
  if (isnan(temperature) || isnan(humidity))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // Check temperature value and control actuator 
  if (temperature > 21.5) {
    // Turn on the pump if it's off
    if (digitalRead(PUMP_PIN) == LOW ) {
      digitalWrite(PUMP_PIN, HIGH);
      digitalWrite(LED,HIGH); // Turn on LED if temperature is above 21.5 degrees
      lastChangeTime = millis();
    }

    // Turn off the pump if it has been on for long enough
    if (millis() - lastChangeTime ==PUMP_ON_DURATION) {
      digitalWrite(PUMP_PIN, LOW);
      digitalWrite(LED, LOW);     // Turn off led otherwise
      delay(5000);    //delay before reading again
    }
  } 


  // Print the percentage value on the serial monitor

  if (millis() - previousTime >= delayTime) {

    
//feed the values to the io
    TEMPERATURE->save(temperature);
    HUMIDITY->save(humidity);

    // Print temperature and humidity
    Serial.print("Temperature Sent is: ");
    Serial.print(temperature);
    Serial.print(" °C\t");
    Serial.print("Humidity Sent is: ");
    Serial.print(humidity);
    Serial.println(" %");
    

    previousTime = millis();

 }
}
