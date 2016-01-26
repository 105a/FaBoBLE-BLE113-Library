//
// FaBo BLE Brick
//
// brick_serial_ble
//
#include <SoftwareSerial.h>
#include "fabo-ble113.h"

void setup()
{
  Serial.begin(9600);
   
  faboBLE.initBLE113();
  faboBLE.setDebug();
  if(faboBLE.setAdvParams()){
    Serial.println("Success:setAdvParams()");
    if(faboBLE.setMode()){
      Serial.println("Success:setMode()");
      byte uuid[16] = {0xcb,0x86,0xbc,0x31,0x05,0xbd,0x40,0xcc,0x90,0x3d,0x1c,0x9b,0xd1,0x3d,0x96,0x6b};
      byte major[2] = {0x00,0x71};
      byte minor[2] = {0x00,0x71};
      faboBLE.setBeaconUuid(uuid);
      faboBLE.setBeaconMajor(major);
      faboBLE.setBeaconMinor(minor);
      if(faboBLE.sendBeacon()){
        Serial.println("Success:Beacon advertising"); 
      } else {
        Serial.println("Failed:Beacon advertising"); 
      }
    } else {
      Serial.println("Failed:setMode()");
    }
  } else {
    Serial.println("Failed:setAdvParams()"); 
  }
}

void loop()
{ 
  
}
