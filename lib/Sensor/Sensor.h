#ifndef SENSOR_H_
#define SENSOR_H_

#include <Arduino.h>
#include <DHT_U.h>

class Sensor {
private:
  static bool sleep;
  static bool sleepBuffer;

public:
  static bool isSleep(void);
  static bool isGoingToSleep(void);
  static void setSleep(bool sleep);

  static void setup(void);
  static void loop(void);

  class DHT_11 {
  private:
    static bool sleep;
    static bool sleepBuffer;

    static unsigned int adc;
    static float temperature;
    static float humidity;
  public:
    static DHT_Unified dht;
    static sensor_t sensor;

    static float getTemperature(void) { return temperature; };
    static float getHumidity(void) { return humidity; };

    static bool isSleep(void);
    static bool isGoingToSleep(void);
    static void setSleep(bool sleep);

    static void setup(void);
    static void loop(void);
  };
};

#endif
