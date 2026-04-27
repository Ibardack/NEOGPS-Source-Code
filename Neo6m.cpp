#include <Arduino.h>
#include <NMEAGPS.h>
#include "Neo6m.h"

static const uint16_t GPS_BAUD_RATE = 9600;
static const uint16_t GPS_SERIAL_READY_TIMEOUT_MS = 1000;
static const uint16_t GPS_FIX_TIMEOUT_MS = 5000;
static const uint8_t GPS_SERIAL_READY_POLL_MS = 5;

gpsInfo currentRead;
NMEAGPS gps;
unsigned long lastValidFixMs = 0;

bool GPS_begin(){
  unsigned long start = millis();
  Serial.begin(GPS_BAUD_RATE);
  while (!Serial && millis() - start < GPS_SERIAL_READY_TIMEOUT_MS) {
    delay(GPS_SERIAL_READY_POLL_MS);
  }
  return true;
}
bool GPS_read(){
  bool newData = false;

  while (gps.available(Serial)) {
    gps_fix fix = gps.read();
    if (fix.valid.location) {
      currentRead.latitudeE7 = fix.latitudeL();
      currentRead.longitudeE7 = fix.longitudeL();
      if (fix.valid.speed) {
        currentRead.speedCmps = (fix.speed_mkn() * 5144L) / 100000L;
      }
      if (fix.valid.altitude) {
        currentRead.altitudeCm = fix.altitude_cm();
      }
      lastValidFixMs = millis();
      newData = true;
    }
  }

  currentRead.valid = lastValidFixMs != 0 && millis() - lastValidFixMs <= GPS_FIX_TIMEOUT_MS;
  return newData;
}
const gpsInfo &GPS_get(){
  return currentRead;
}
