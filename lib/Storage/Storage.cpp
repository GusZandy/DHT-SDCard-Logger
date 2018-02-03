#include "Config.h"
#include "Indicating.h"
#include "Storage.h"
#include "Sensor.h"

File Storage::SdCard::file;

bool Storage::SdCard::initialized = false;

void Storage::setup(void) {
  SdCard::setup();
}

void Storage::loop(void) {
  SdCard::loop();
}

/* Public */
void Storage::SdCard::setup(void) {

  pinMode(SD_SS_PIN, OUTPUT);
  digitalWrite(SD_SS_PIN, HIGH);
  delay(1000);
  if (!SD.begin(SD_SS_PIN)) return;
  Storage::SdCard::writeFile((char*) "log", " ", strlen(" "));
  initialized = true;
}

void Storage::SdCard::loop(void) {
  if (!initialized) {
    Serial.print("Trtying to initialized... ");
    if (!SD.begin(SD_SS_PIN)) {
      Serial.println("failed");
      return;
    }
    Serial.println("succes");
    initialized = true;
  }
}

bool Storage::SdCard::writeFile(char *filepath, const char *data, size_t size) {
  if (!initialized) return false;
  file = SD.open(filepath, FILE_WRITE);
  if(file) {
    file.println(data);
    file.close();
    Serial.println(data);
    return true;
  } else return false;
}

const char *Storage::SdCard::implode(char buffer[], char timestamp[], float temperature, float humidity, const char *delimiter) {
  // 14:25 25:01:2018,00.00,00.00
  char valueBuffer[10];
  strcpy(buffer, timestamp); //16
  strcat(buffer, delimiter); //1
  strcat(buffer, dtostrf(temperature, 1, 2, valueBuffer)); //5
  strcat(buffer, delimiter); //1
  strcat(buffer, dtostrf(humidity, 1, 2, valueBuffer)); //5

  return buffer;
}
