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
  volatile static bool hasWritten = false;

  System::loop();
  Indicating::loop();
  Sensor::loop();
  Indicating::LCD::measurement(Indicating::RTC::getHour(), Indicating::RTC::getMinute(), Indicating::RTC::getHalfSecond(), Storage::SdCard::isInitialized(), Sensor::DHT_11::getTemperature(), Sensor::DHT_11::getHumidity());

  static unsigned long lastSleep = millis();
  static bool isTimeout = false;
  unsigned int currentMinuteInDay = ((unsigned int) Indicating::RTC::getHour() * 60) + (unsigned int) Indicating::RTC::getMinute();
  isTimeout = (millis() - lastSleep > 60000);

  Storage::loop();
  if (((currentMinuteInDay % LOGGING_INTERVAL) == 0)) {
    if (Storage::SdCard::isInitialized() && !hasWritten && (Sensor::DHT_11::getTemperature() != 0.00f || Sensor::DHT_11::getHumidity() != 0.00f)) {
      char buffer[35];
      Storage::SdCard::implode(buffer, (char *) Indicating::RTC::getDateTimeSQLFormat(), Sensor::DHT_11::getTemperature(), Sensor::DHT_11::getHumidity(), ",");
      Storage::SdCard::writeFile((char*) "data", buffer, strlen(buffer));
      hasWritten = true;
    }
    return;
  }
  hasWritten = false;

  if (isTimeout) {
    Indicating::LCD::idle();
  }
}
