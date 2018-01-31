#include "Config.h"
#include "Indicating.h"
#include "Storage.h"
#include "Sensor.h"

File Storage::SdCard::file;

bool Storage::SdCard::initialized = false;

/* Public */
void Storage::SdCard::setup(void) {
  // Indicating::Display::splash(F("Init SD Card"));

  pinMode(SD_SS_PIN, OUTPUT);
  digitalWrite(SD_SS_PIN, HIGH);
  if (!SD.begin(SD_SS_PIN)) {
    // Indicating::Display::splash(F("Init SD Gagal"));
    delay(3000);
    return;
  }
  initialized = true;
}

void Storage::SdCard::loop(void) {
  if (!initialized) {
    if (!SD.begin(SD_SS_PIN)) {
      return;
    }
    initialized = true;
  }
}

bool Storage::SdCard::writeFile(char *filepath, const char *data, size_t size) {
  if (!initialized) return false;
  file = SD.open(filepath, FILE_WRITE);
  if(file) {
    file.println(data);
    file.close();
    return true;
  } else return false;
  return true;
}

void Storage::SdCard::readFile(char *buffer, char *filepath, size_t expectedSize) {
  readFile((void *) buffer, filepath, expectedSize);
}

void Storage::SdCard::readFile(void *buffer, char *filepath, size_t expectedSize) {
  if (!initialized) return false;
  if (!SD.exists(filepath)) return;
  file = SD.open(filepath);
  if (0 < expectedSize && expectedSize != file.size()) return;
  file.readBytes((unsigned char *) buffer, file.size());
}

bool Storage::SdCard::isDirectoryEmpty(char *path) {
  if (!initialized) return false;
  if (!SD.exists(path)) return false;
  File folder = SD.open(path);
  if (!folder.isDirectory()) return false;
  return !folder.openNextFile();
}

const char *Storage::SdCard::implode(char buffer[], const struct Data *data, const char *delimiter) {
  // 14:25 25:01:2018,00.00,00.00
  char valueBuffer[10];
  strcpy(buffer, data->timestamp); //16
  strcat(buffer, delimiter); //1
  strcat(buffer, dtostrf(data->temperature, 1, 2, valueBuffer)); //5
  strcat(buffer, delimiter); //1
  strcat(buffer, dtostrf(data->humidity, 1, 2, valueBuffer)); //5

  return buffer;
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
