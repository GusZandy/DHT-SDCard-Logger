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
  // Suhu::setup();
  DHT_11::setup();
  Indicating::LCD::bootInfo("Init Sensor.", 3);
}

void Sensor::loop(void) {
  // Suhu::loop();
  DHT_11::loop();
}

bool Sensor::Suhu::sleep = false;
bool Sensor::Suhu::sleepBuffer = false;

unsigned int Sensor::Suhu::adc;
float Sensor::Suhu::temperature;

void Sensor::Suhu::setup(void) {
  pinMode(LM35_PIN, INPUT);
}

void Sensor::Suhu::loop(void) {
  if (sleepBuffer != sleep) {
    sleep = sleepBuffer;
  }
  char valueBuffer[10];
  if (sleep) return;

  static unsigned long measurementTime = millis() - MEASUREMENT_INTERVAL;

  if (millis() - measurementTime >= MEASUREMENT_INTERVAL) {
    adc = analogRead(LM35_PIN);
    for (size_t i = 0; i < 1000; i++) {
      temperature += (3.30 *  (adc) * 100.00) / 1024.00; //dikurangi 10 hasil kalibrasi di 0 volt
    }
    temperature = temperature / 1000.00;
    measurementTime = millis();
  }

  System::Debugging::println("temperature: %s", dtostrf(temperature, 1, 2, valueBuffer));
}

bool Sensor::DHT_11::sleep = false;
bool Sensor::DHT_11::sleepBuffer = false;

// unsigned int Sensor::DHT::adc;
float Sensor::DHT_11::temperature;
float Sensor::DHT_11::humidity;

DHT_Unified Sensor::DHT_11::dht(DHT_PIN, DHT_TYPE);
sensor_t Sensor::DHT_11::sensor;
struct Data Sensor::DHT_11::data;

struct Data &Sensor::DHT_11::getDHTData(void) {
  strcpy(data.timestamp, Indicating::RTC::getDateTimeSQLFormat());
  data.temperature = getTemperature();
  data.humidity = getHumidity();

  return data;
}

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
