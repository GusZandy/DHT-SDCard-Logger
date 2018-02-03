#ifndef SENSOR_H_
#define SENSOR_H_

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

struct Data {
  // xx:xx xx-xx-xxxx
  char timestamp[18];
  float temperature;
  float humidity;
};

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

  class Suhu {
  private:
    static bool sleep;
    static bool sleepBuffer;

    static unsigned int adc;
    static float temperature;
  public:
    static float getTemperature(void) { return temperature; };
    static unsigned int getAdc(void) { return adc; };

    static bool isSleep(void);
    static bool isGoingToSleep(void);
    static void setSleep(bool sleep);

    static void setup(void);
    static void loop(void);
  };

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
    static struct Data data;

    static struct Data &getDHTData(void);
    static float getTemperature(void) { return temperature; };
    static float getHumidity(void) { return humidity; };
    static unsigned int getAdc(void) { return adc; };

    static bool isSleep(void);
    static bool isGoingToSleep(void);
    static void setSleep(bool sleep);

    static void setup(void);
    static void loop(void);
  };
};

#endif
