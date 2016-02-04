#include "Arduino.h"
#include <SoftwareSerial.h>

// SCANデータ格納用構造体
typedef struct {
  int8_t rssi;
  byte packettype;
  byte sender[7];
  byte addrtype;
  byte bond;
  int data_len;
  byte data[255];
}typeScanData;

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

  bool scan(typeScanData *data);
  // Ble初期化
  void init();
  // set scan parameters
  bool setScanParams(byte param[]);
  // Scan開始
  bool scanStart();
  int count();
  // 出力処理(16進数２桁)
  void hex_output(byte out_byte);
};

struct BeaconParam {
  byte uuid[16];
  byte major[2];
  byte minor[2];
};


extern ble113 faboBLE;
