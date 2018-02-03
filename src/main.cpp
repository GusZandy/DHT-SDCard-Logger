#include <Arduino.h>
#include "Config.h"
#include "Sensor.h"
#include "System.h"
#include "Indicating.h"
#include "Storage.h"

#include <EnableInterrupt.h>

void setup() {
  System::setup();
  Indicating::setup();
  Indicating::LCD::splash();
  Storage::setup();
  Sensor::setup();
}

void loop() {
  struct Data *temp;
  volatile static bool hasWritten = false;
  temp = &Sensor::DHT_11::getDHTData();

  System::loop();
  Indicating::loop();
  Sensor::loop();
  Indicating::LCD::measurement(Indicating::RTC::getHour(), Indicating::RTC::getMinute(), Indicating::RTC::getHalfSecond(), 0, temp->temperature, temp->humidity);
  // Indicating::LCD::measurement(Indicating::RTC::getHour(), Indicating::RTC::getMinute(), Indicating::RTC::getHalfSecond(), 0, Sensor::DHT_11::getTemperature(), Sensor::DHT_11::getHumidity());


  static unsigned long lastSleep = millis();
  static bool isTimeout = false;
  unsigned int currentMinuteInDay = ((unsigned int) Indicating::RTC::getHour() * 60) + (unsigned int) Indicating::RTC::getMinute();
  isTimeout = (millis() - lastSleep);

  Storage::loop();
  if (((currentMinuteInDay % LOGGING_INTERVAL) == 0)) {
    if (Storage::SdCard::isinitialized() && !hasWritten && (temp->temperature != 0.00f || temp->humidity != 0.00f)) {
      char buffer[35];
      temp = &Sensor::DHT_11::getDHTData();
      Storage::SdCard::implode(buffer, temp, ",");
      Storage::SdCard::writeFile((char*) "log", buffer, strlen(buffer));
      hasWritten = true;
    }
    return;
  }
  hasWritten = false;

  if (isTimeout) {
    /* code */
  }
}
