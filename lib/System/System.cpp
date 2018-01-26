#include "Config.h"
#include "System.h"

bool System::Debugging::debug = SYSTEM_DEBUG;

bool System::sleep = false;
bool System::sleepBuffer = false;

void System::setSleep(bool sleep) {
  sleepBuffer = sleep;
}

void System::setup(void) {
  Debugging::setup();
}

void System::loop(void) {
  Debugging::loop();
}

void System::Debugging::setup(void) {
  SYSTEM_SERIAL.begin(SYSTEM_SERIAL_BAUDRATE);
  if (!debug) return;

  print("Debug Mode\r\n");
}

void System::Debugging::loop(void) {
  if (!debug) return;
  SYSTEM_SERIAL.flush();
}

void System::Debugging::print(const char fmt[], ...) {
  if (!debug) return;
  char buffer[256];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  va_end(args);
  SYSTEM_SERIAL.print(buffer);
  SYSTEM_SERIAL.flush();
}

void System::Debugging::println(const char fmt[], ...) {
  if (!debug) return;
  char buffer[256];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  va_end(args);
  SYSTEM_SERIAL.println(buffer);
  SYSTEM_SERIAL.flush();
}
