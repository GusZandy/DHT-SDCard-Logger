#ifndef STORAGE_H_
#define STORAGE_H_

#include <Arduino.h>
#include <SD.h>

class Storage {
public:
  static void setup(void);
  static void loop(void);

  class SdCard {
  private:
    static File file;
    static bool initialized;

  public:
    static void setup(void);
    static void loop(void);

    static bool writeFile(char *filepath, const char *data, size_t size);
    static bool isInitialized(void) { return initialized; };

    static const char *implode(char buffer[], char timestamp[], float temperature, float humidity, const char *delimiter);
  };
};



#endif
