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
  Sensor::setup();
}

void loop() {
  System::loop();
  Indicating::loop();
  Sensor::loop();
  Indicating::LCD::measurement(Indicating::RTC::getHour(), Indicating::RTC::getMinute(), Indicating::RTC::getHalfSecond(), 0, Sensor::DHT_11::getTemperature(), Sensor::DHT_11::getHumidity());

  static unsigned long lastSleep = millis();
  static bool isTimeout = false;
  unsigned int currentMinuteInDay = ((unsigned int) Indicating::RTC::getHour() * 60) + (unsigned int) Indicating::RTC::getMinute();
  isTimeout = (millis() - lastSleep);

  if ((currentMinuteInDay % LOGGING_INTERVAL) == 0) {
    char buffer[30];
    Storage::SdCard::implode(buffer, &Sensor::DHT_11::getDHTData(), (const char*) ',');
    Storage::SdCard::writeFile((char*) "DHT_LOGGER", Storage::SdCard::implode(buffer, &Sensor::DHT_11::getDHTData(), (const char*) ','), strlen(buffer));
    // Storage::SdCard::implode(buffer, (char *)Indicating::RTC::getDateTimeSQLFormat(), Sensor::DHT_11::getTemperature(), Sensor::DHT_11::getHumidity(), (const char*) ',');
  }
  if (isTimeout) {
    /* code */
  }
}
