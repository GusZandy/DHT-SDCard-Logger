#ifndef CONFIG_H_
#define CONFIG_H_

/*== System properties ==*/
#define SYSTEM_DEBUG false
#define SYSTEM_SERIAL Serial
#define SYSTEM_SERIAL_BAUDRATE 115200

/*== LM35 properties ==*/
#define LM35_PIN A0

/*== RTC properties ==*/
#define RTC_SQWO_PIN 3

/*== DHT properties ==*/
#define DHT_ENABLE true

#ifdef DHT_ENABLE
#define DHT_TYPE DHT11
#define DHT_PIN 5
#endif

/*==SD #ARD==*/
#define SD_SS_PIN 4

#define MEASUREMENT_INTERVAL 1000   //in miliseconds
#define LOGGING_INTERVAL 2         //in Minutes

#endif
