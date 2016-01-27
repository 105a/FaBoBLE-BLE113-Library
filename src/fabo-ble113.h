#include "Arduino.h"
#include <SoftwareSerial.h>

class ble113
{
public:
  void setDebug(void);
  void initBLE113(void);
  bool setAdvParams(void);
  bool setMode(void);
  void setBeaconUuid(byte uuid[]);
  void setBeaconMajor(byte major[]);
  void setBeaconMinor(byte minor[]);
  bool sendBeacon(void);
  bool stopAdv(void);
  bool isRunning(void);
};

struct BeaconParam {
  byte uuid[16];
  byte major[2];
  byte minor[2];
};


extern ble113 faboBLE;
