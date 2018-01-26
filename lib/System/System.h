#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <Arduino.h>

class System {
private:
  static bool sleep;
  static bool sleepBuffer;

public:
  static void setup(void);
  static void loop(void);

  static void initPin(void);

  static bool isSleep(void);
  static bool isGoingToSleep(void);
  static void setSleep(bool sleep);

  class Debugging {
  private:
    static bool debug;

  public:
    static void print(const char fmt[], ...);
    static void println(const char fmt[], ...);

    static void setup(void);
    static void loop(void);
  };
};

#endif
