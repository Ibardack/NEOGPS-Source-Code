#ifndef NEO6M_H
#define NEO6M_H

struct gpsInfo {
  long latitudeE7;
  long longitudeE7;
  long speedCmps;
  long altitudeCm;
  bool valid = false;
};


bool GPS_begin();
bool GPS_read();
const gpsInfo &GPS_get();

#endif
