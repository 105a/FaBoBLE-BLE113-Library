//
// FaBo BLE Brick
//
// brick_serial_ble
//
#include <SoftwareSerial.h>
#include "fabo-ble113.h"

#define buttonPin A0 // ボタンピン

// ボタンの押下状況取得用
int buttonState = 0;
bool isFirst = false;
FaBoBLE faboBLE;

void setup()
{
  Serial.begin(9600);

  // ボタンピンを入力用に設定
  pinMode(buttonPin, INPUT); 
  faboBLE.setDebug();
  faboBLE.init();
  
  if(faboBLE.setAdvParameters()){
    Serial.println("Success:setAdvParams()");
      byte uuid[16] = {0xcb,0x86,0xbc,0x31,0x05,0xbd,0x40,0xcc,0x90,0x3d,0x1c,0x9b,0xd1,0x3d,0x96,0x6b};
      byte major[2] = {0x00,0x71};
      byte minor[2] = {0x00,0x71};
      faboBLE.setBeaconUuid(uuid);
      faboBLE.setBeaconMajor(major);
      faboBLE.setBeaconMinor(minor);

    if(faboBLE.sendBeacon()){
      Serial.println("Success:sendBeacon()");
    } else{
      Serial.println("Failed:sendBeacon");
    }
  } else {
    Serial.println("Failed:setAdvParams()"); 
  }

  if(faboBLE.startAdv()){
    Serial.println("Success:Start Beacon advertising"); 
  } else {
    Serial.println("Failed:Start Beacon advertising"); 
  }

}

void loop()
{ 
  // ボタンの押下状況を取得
  buttonState = digitalRead(buttonPin);
  
  // ボタン押下判定
  if (buttonState == HIGH && isFirst == false) {
      isFirst == true;
      if(!faboBLE.isAdvertising()){
        if(faboBLE.startAdv()){
          Serial.println("Success:Start Beacon advertising"); 
        } else {
          Serial.println("Failed:Start Beacon advertising"); 
        }
      }  
      else if(faboBLE.isAdvertising()){     
        if(faboBLE.stopAdv()){
          Serial.println("Success:Stop Beacon advertising "); 
        } else {
          Serial.println("Failed:Stop Beacon advertising"); 
        }   
      }
  } else {
    isFirst = false;
  }
}

