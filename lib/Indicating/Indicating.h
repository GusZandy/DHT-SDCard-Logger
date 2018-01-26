#ifndef INDICATING_H_
#define INDICATING_H_

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>

class Indicating {
private:
  static bool sleep;
  static bool sleepBuffer;

public:
  static void setup(void);
  static void loop(void);

  static void initPin(void);

  static bool isSleep(void);
  static bool isGoingToSleep(void);
  static bool isGoingToWake(void);
  static void setSleep(bool sleep);

  class RTC {
  private:
    static bool sleep;
    static bool sleepBuffer;

    static RTC_DS3231 ds3231;
    static DateTime dateTime;
    static DateTime dateTimeBuffer;
    static bool dateTimeChanged;

    static int timezone;
    static volatile bool halfSecond;
    static volatile unsigned int secondCount;

  public:
    static void setup();
    static void loop();

    static DateTime getDateTime(void) { return dateTime; };
    static void setDateTimeSQLFormat(const char *dateTimeGSMFormat);
    static const char *getDateTimeSQLFormat(void);
    static const unsigned char getHour(void) { return dateTime.hour(); };
    static const unsigned char getMinute(void) { return dateTime.minute(); };
    static bool getHalfSecond(void) { return halfSecond; };

    static void onChangeSQWO(void);

    static bool isSleep(void);
    static bool isGoingToSleep(void);
    static bool isGoingToWake(void);
    static void setSleep(bool sleep);
  };

  class LCD {
  private:
    static bool debug;

  public:
    static LiquidCrystal_I2C lcd;
    static void print(const char fmt[], ...);
    static void println(const char fmt[], ...);

    static void setup(void);
    static void loop(void);

    static void idle(void);
    static void wakeUp(void);
    static void bootInfo(const char* text, const int x);
    static void splash(void);
    static void measurement(unsigned char hour, unsigned char minute, bool doubleDots, unsigned char status, float temperature);
    static void measurement(unsigned char hour, unsigned char minute, bool doubleDots, unsigned char status, float temperature, float humidity);
  };
};

#endif
