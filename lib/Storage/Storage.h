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
    static void readFile(char *buffer, char *filepath, size_t expectedSize = 0);
    static void readFile(void *buffer, char *filepath, size_t expectedSize = 0);
    static bool isDirectoryEmpty(char *path);
    static bool isinitialized(void) { return initialized; };

    static const char *implode(char buffer[], const struct Data *data, const char *delimiter);
    static const char *implode(char buffer[], char timestamp[], float temperature, float humidity, const char *delimiter);
  };
};



#endif
