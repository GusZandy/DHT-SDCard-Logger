#include "Sensor.h"
#include "Config.h"
#include "System.h"
#include "DHT.h"
#include "Indicating.h"

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

bool Sensor::sleep = false;
bool Sensor::sleepBuffer = false;

void Sensor::setSleep(bool sleep) {
  sleepBuffer = sleep;
}

void Sensor::setup(void) {
  DHT_11::setup();
  Indicating::LCD::bootInfo("Init Sensor.", 2);
}

void Sensor::loop(void) {
  DHT_11::loop();
}

bool Sensor::DHT_11::sleep = false;
bool Sensor::DHT_11::sleepBuffer = false;

// unsigned int Sensor::DHT::adc;
float Sensor::DHT_11::temperature;
float Sensor::DHT_11::humidity;

DHT_Unified Sensor::DHT_11::dht(DHT_PIN, DHT_TYPE);
sensor_t Sensor::DHT_11::sensor;

void Sensor::DHT_11::setup(void) {
  dht.begin();
}

void Sensor::DHT_11::loop(void) {
  if (sleepBuffer != sleep) {
    sleep = sleepBuffer;
  }
  char temperatureBuffer[10], humidityBuffer[10];
  if (sleep) return;

  static unsigned long measurementTime = millis() - MEASUREMENT_INTERVAL;

  if (millis() - measurementTime >= MEASUREMENT_INTERVAL) {
    // delay(delayMS);
    // Get temperature event and print its value.
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature)) {
      System::Debugging::println("Error reading temperature");
    }
    else {
      temperature = event.temperature;
    }
    // Get humidity event and print its value.
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
      System::Debugging::println("Error reading humidity");
    }
    else {
      humidity = event.relative_humidity;
    }
    measurementTime = millis();
    System::Debugging::println("temperature: %s, humidity: %s", dtostrf(temperature, 1, 2, temperatureBuffer),dtostrf(humidity, 1, 2, humidityBuffer));
  }

}
