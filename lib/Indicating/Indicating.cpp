#define LIBCALL_ENABLEINTERRUPT

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EnableInterrupt.h>

#include "Config.h"
#include "Indicating.h"
#include "System.h"

bool Indicating::sleep = false;
bool Indicating::sleepBuffer = false;

void Indicating::setSleep(bool sleep) {
  sleepBuffer = sleep;
}

bool Indicating::isGoingToSleep(void) {
  return sleepBuffer & !sleep;
}

bool Indicating::isGoingToWake(void) {
  return !sleepBuffer & sleep;
}

void Indicating::setup(void) {
  RTC::setup();
  LCD::setup();
}

void Indicating::loop(void) {
  RTC::loop();
  LCD::loop();
}

RTC_DS3231 Indicating::RTC::ds3231;
DateTime Indicating::RTC::dateTime;
DateTime Indicating::RTC::dateTimeBuffer;

bool Indicating::RTC::sleep = false;
bool Indicating::RTC::sleepBuffer = false;

int Indicating::RTC::timezone = 0;
bool Indicating::RTC::dateTimeChanged = false;
volatile bool Indicating::RTC::halfSecond = false;
volatile unsigned int Indicating::RTC::secondCount = 0;

void Indicating::RTC::setup(void) {
  pinMode(RTC_SQWO_PIN, INPUT_PULLUP);

  if (!ds3231.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  // ds3231.adjust(DateTime(2018, 1, 22, 21, 28,+ 0));
  // if (!ds3231.isrunning()) {
  //   ds3231.adjust(DateTime(0, 1, 1, 0, 0, 0));
  // }
  ds3231.writeSqwPinMode(DS3231_SquareWave1Hz);
  dateTime = ds3231.now();
  secondCount = 0;
  enableInterrupt(RTC_SQWO_PIN, Indicating::RTC::onChangeSQWO, CHANGE);
}

void Indicating::RTC::loop(void) {
  if (sleepBuffer != sleep) {
    sleep = sleepBuffer;
  }

  static unsigned long startReading= millis();

  if (millis() - startReading >= 10000) {
    dateTime = ds3231.now();
    startReading = millis();
  }
}

bool Indicating::RTC::isSleep(void) {
  return sleep;
}

void Indicating::RTC::setSleep(bool sleep) {
  sleepBuffer = sleep;
}

void Indicating::RTC::setDateTimeSQLFormat(const char *dateTimeSQLFormat) {
  char dateTimeSQLFormatBuffer[strlen(dateTimeSQLFormat) + 1];
  int year, month, day, hour, minute, second;

  strcpy(dateTimeSQLFormatBuffer, dateTimeSQLFormat);
  year = atoi(strtok(dateTimeSQLFormatBuffer, " /,:+-"));
  month = atoi(strtok(NULL, " /,:+-"));

  day = atoi(strtok(NULL, "  /,:+-"));
  hour = atoi(strtok(NULL, " /,:+-"));
  minute = atoi(strtok(NULL, " /,:+-"));
  second = atoi(strtok(NULL, " /,:+-"));
  timezone = atoi(dateTimeSQLFormat + 17) / 4;

  dateTimeBuffer = DateTime(year, month, day, hour, minute, second);
  secondCount = 0;
  dateTimeChanged = true;
}

const char *Indicating::RTC::getDateTimeSQLFormat(void) {
  static char dateTimeSQLFormatBuffer[17];

  sprintf_P(dateTimeSQLFormatBuffer, (const char*) F("%.2d:%.2d %d-%d-%d"), dateTime.hour(), dateTime.minute(), dateTime.day(), dateTime.month(), dateTime.year());
  return dateTimeSQLFormatBuffer;
}

/**
 * Interrupt Routine
 */
void Indicating::RTC::onChangeSQWO(void) {
  halfSecond = digitalRead(RTC_SQWO_PIN);
  if (halfSecond) {
    return;
  }
  secondCount++;
}

LiquidCrystal_I2C Indicating::LCD::lcd(0x27, 16, 2);

void Indicating::LCD::setup(void) {
  System::Debugging::println("Setup LCD...");
  lcd.begin();
  lcd.clear();
}

void Indicating::LCD::loop(void) {
  if (sleepBuffer != sleep) {
  sleep = sleepBuffer;
  }
  if (sleep) {
    idle();
    return;
  } else if (isGoingToWake()){
    wakeUp();
  }
}

void Indicating::LCD::idle(void) {
  lcd.noDisplay();
  lcd.noBacklight();
}

void Indicating::LCD::wakeUp(void) {
  lcd.display();
  lcd.backlight();
}

void Indicating::LCD::bootInfo(const char *text, const int x) {
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("Id-IoT");
  lcd.setCursor(x,1);
  lcd.print(text);
  delay(3000);
  lcd.clear();
}

void Indicating::LCD::splash(void) {
  lcd.setCursor(5,0);
  lcd.print("Hello!");
  lcd.setCursor(1,1);
  lcd.print("Good morning..");
  delay(3000);
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("Id-IoT");
  delay(3000);
  lcd.clear();
}

void Indicating::LCD::measurement(unsigned char hour, unsigned char minute, bool doubleDots, unsigned char status, float temperature) {
  char buffer[7], valueBuffer[10];

  //status
  lcd.setCursor(0, 0);

  //clock
  if (doubleDots) {
    sprintf_P(buffer, (const char *)("%.2d:%.2d"), hour, minute);
    lcd.setCursor(11, 0);
    lcd.print(buffer);
  } else {
    sprintf_P(buffer, (const char *)("%.2d %.2d"), hour, minute);
    lcd.setCursor(11, 0);
    lcd.print(buffer);
  }

  //data
  sprintf_P(buffer, (const char *)("Temp: %s%cC"), dtostrf(temperature, 1, 1, valueBuffer), 223);
  lcd.setCursor(0, 1);
  lcd.print(buffer);
}

void Indicating::LCD::measurement(unsigned char hour, unsigned char minute, bool doubleDots, unsigned char status, float temperature, float humidity) {
  char buffer[7], temperatureBuffer[10], humidityBuffer[10];

  //status
  lcd.setCursor(0, 0);

  //clock
  if (doubleDots) {
    sprintf_P(buffer, (const char *)F("%.2d:%.2d"), hour, minute);
    lcd.setCursor(11, 0);
    lcd.print(buffer);
  } else {
    sprintf_P(buffer, (const char *)F("%.2d %.2d"), hour, minute);
    lcd.setCursor(11, 0);
    lcd.print(buffer);
  }

  //data
  sprintf_P(buffer, (const char *)F("Tem:%s%cC Hum:%s%c"), dtostrf(temperature, 1, 0, temperatureBuffer), 223, dtostrf(humidity, 1, 0, humidityBuffer), 37);
  lcd.setCursor(0, 1);
  lcd.print(buffer);
}
