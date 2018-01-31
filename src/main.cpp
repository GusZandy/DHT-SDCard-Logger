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
  struct Data *temp;
  static bool hasWritten = false;

  System::loop();
  Indicating::loop();
  Sensor::loop();
  Indicating::LCD::measurement(Indicating::RTC::getHour(), Indicating::RTC::getMinute(), Indicating::RTC::getHalfSecond(), 0, temp->temperature, temp->humidity);

  temp = &Sensor::DHT_11::getDHTData();

  static unsigned long lastSleep = millis();
  static bool isTimeout = false;
  unsigned int currentMinuteInDay = ((unsigned int) Indicating::RTC::getHour() * 60) + (unsigned int) Indicating::RTC::getMinute();
  isTimeout = (millis() - lastSleep);

  if (((currentMinuteInDay % LOGGING_INTERVAL) == 0)) {
    if (Storage::SdCard::isinitialized() && !hasWritten) {
      char buffer[30];
      Storage::SdCard::implode(buffer, temp, ",");
      Storage::SdCard::writeFile((char*) "DHT_LOGGER", Storage::SdCard::implode(buffer, &Sensor::DHT_11::getDHTData(), (const char*) ','), strlen(buffer));
      hasWritten = true;
    }
    return;
  }
  hasWritten = false;
  
  if (isTimeout) {
    /* code */
  }
}
