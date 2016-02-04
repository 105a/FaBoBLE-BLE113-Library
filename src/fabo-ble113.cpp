#include "fabo-ble113.h"
#include <SoftwareSerial.h>
#include <fabo-queue.h>

queue myQueue;
BeaconParam beaconParam;

SoftwareSerial bleShield(12, 13);
bool DEBUG = false;
bool isRunningFlag = false;
int buff_count = 0;

void ble113::setDebug(){
    DEBUG = true;
}

void ble113::initBLE113()
{
    // BLE縺ｨ縺ｮ騾壻ｿ｡逕ｨ
    bleShield.begin(9600);
    
    if(DEBUG){
        Serial.begin(9600);
    }
}

void ble113::setBeaconUuid(byte uuid[]){
    
    memcpy(beaconParam.uuid, uuid, 16);
}

void ble113::setBeaconMajor(byte major[]){
    
    memcpy(beaconParam.major, major, 2);
}

void ble113::setBeaconMinor(byte minor[]){
    
    memcpy(beaconParam.minor, minor, 2);
}

bool ble113::sendBeacon(){
    
    byte command[6] = {0x00,  // message type  ->0x00:command
        0x20,     // Minimum payload length
        0x06,     // Message class -> 0x06:Generic Access Profile
        0x09,     // Message ID
        0x00,     // Advertisement data type -> 0x00: sets advertisement data e
        0x1e};    // Advertisement data to send
    
    byte beaconHeader[9] = {0x02,
        0x01,
        0x06,
        0x1A,
        0xFF,
        0x4C,
        0x00,
        0x02,
        0x15};
    byte txPower = 0xC9;
    
    for(int i = 0; i < 6; i++){
        bleShield.write((byte)command[i]);
    }
    for(int i = 0; i < 9; i++){
        bleShield.write((byte)beaconHeader[i]);
    }
    for(int i = 0; i < 16; i++){
        bleShield.write((byte)beaconParam.uuid[i]);
    }
    for(int i = 0; i < 2; i++){
        bleShield.write((byte)beaconParam.major[i]);
    }
    for(int i = 0; i < 2; i++){
        bleShield.write((byte)beaconParam.minor[i]);
    }
    
    bleShield.write((byte)0xC9);
    
    delay(1000);
    
    byte buffer[10];
    int i = 0;
    while (bleShield.available()) {
        buffer[i] = bleShield.read();
        i++;
    }
    
    if(buffer[0] == 0x00 && buffer[1] == 0x02 && buffer[2] == 0x06 && buffer[3] == 0x09){
        if(buffer[4] == 0x00 && buffer[5] == 0x00){
            isRunningFlag = true;
          
            return true;
        }
        else {
            if(DEBUG){
                Serial.println("(sendBeacon)Error Code");
                Serial.println(buffer[5],HEX);
                Serial.println(buffer[4],HEX);
            }
            return false;
        }
    } else {
        if(DEBUG){
            Serial.println("(sendBeacon)Unknow Error");
            Serial.println(buffer[0],HEX);
            Serial.println(buffer[1],HEX);
            Serial.println(buffer[2],HEX);
            Serial.println(buffer[3],HEX);
            Serial.println(buffer[4],HEX);
            Serial.println(buffer[5],HEX);
        }
        return false;
    }
}

bool ble113::setMode()
{
    
    byte command[6] = {0x00,  // message type  ->0x00:command
        0x02,     // Minimum payload length
        0x06,     // Message class -> 0x06:Generic Access Profile
        0x01,     // Message ID
        0x04,     // GAP Discoverable Mode
        0x00};    // GAP Connectable Mode
    
    for(int i = 0; i < 6; i++){
        bleShield.write((byte)command[i]);
    }
    delay(1000);
    
    byte buffer[10];
    int i = 0;
    while (bleShield.available()) {
        buffer[i] = bleShield.read();
        i++;
    }
    
    if(buffer[0] == 0x00 && buffer[1] == 0x02 && buffer[2] == 0x06 && buffer[3] == 0x01){
        if(buffer[4] == 0x00 && buffer[5] == 0x00){
            
            return true;
        }
        else {
            if(DEBUG){
                Serial.println("Error Code");
                Serial.println(buffer[5],HEX);
                Serial.println(buffer[4],HEX);
            }
            return false;
        }
    } else {
        if(DEBUG){
            Serial.println("Unknow Error");
            Serial.println(buffer[0],HEX);
            Serial.println(buffer[1],HEX);
            Serial.println(buffer[2],HEX);
            Serial.println(buffer[3],HEX);
            Serial.println(buffer[4],HEX);
            Serial.println(buffer[5],HEX);
        }
        return false;
    }
}


bool ble113::setAdvParams()
{
    byte command[9] = {0x00,
        0x05,
        0x06,
        0x08,
        0x00,
        0x02,
        0x00,
        0x02,
        0x07};
    
    for(int i = 0; i < 9; i++){
        bleShield.write((byte)command[i]);
    }
    delay(1000);
    
    byte buffer[10];
    int i = 0;
    while (bleShield.available()) {
        buffer[i] = bleShield.read();
        i++;
    }
    
    if(buffer[0] == 0x00 && buffer[1] == 0x02 && buffer[2] == 0x06 && buffer[3] == 0x08){
        if(buffer[4] == 0x00 && buffer[5] == 0x00){
            
            return true;
        }
        else {
            if(DEBUG){
                Serial.println("Error Code");
                Serial.println(buffer[5],HEX);
                Serial.println(buffer[4],HEX);
            }
            return false;
        }
    } else {
        if(DEBUG){
            Serial.println("Unknow Error");
            Serial.println(buffer[0],HEX);
            Serial.println(buffer[1],HEX);
            Serial.println(buffer[2],HEX);
            Serial.println(buffer[3],HEX);
            Serial.println(buffer[4],HEX);
            Serial.println(buffer[5],HEX);
        }
        return false;
    }
}


bool ble113::stopAdv()
{

  byte command[6] = {0x00,  // message type  ->0x00:command
                  0x02,     // Minimum payload length
                  0x06,     // Message class -> 0x06:Generic Access Profile
                  0x01,     // Message ID
                  0x00,     // GAP Discoverable Mode
                  0x00};    // GAP Connectable Mode

    for(int i = 0; i < 6; i++){ 
      bleShield.write((byte)command[i]);
    }
    delay(1000);

    byte buffer[10];
    int i = 0;
    while (bleShield.available()) {
      buffer[i] = bleShield.read();
      i++;
    }

    if(buffer[0] == 0x00 && buffer[1] == 0x02 && buffer[2] == 0x06 && buffer[3] == 0x01){
      if(buffer[4] == 0x00 && buffer[5] == 0x00){
        isRunningFlag = false;
        return true;
      }
      else {
        if(DEBUG){
          Serial.println("Error Code");
          Serial.println(buffer[5],HEX);
          Serial.println(buffer[4],HEX);
        }
        return false;
      }
    } else {
      if(DEBUG){
          Serial.println("Unknow Error");
          Serial.println(buffer[0],HEX);
          Serial.println(buffer[1],HEX);
          Serial.println(buffer[2],HEX);
          Serial.println(buffer[3],HEX);
          Serial.println(buffer[4],HEX);
          Serial.println(buffer[5],HEX);
        }
      return false;
    }
}

bool ble113::scan(typeScanData *data) {
  int i = 0;
  int timeout = 100;
  // 取得処理
  long time = millis();
  long timeoutEnd = time + timeout;
  buff_count =0;
  uint8_t read_data[255];
  int len;
  int data_count =0;
  bool end_flg = false;
  int scan_len = 4;

  while(!end_flg){
    if(bleShield.overflow()){
	  Serial.println("Serial Over Flow!!!!!");
	  bleShield.flush();
	  myQueue.flush();
	}

    // シリアルから読み取ったデータはすぐにQueueに入れる
    while (bleShield.available()) {
      myQueue.push(bleShield.read());
    }
    if((myQueue.size() >= 255)){
	  Serial.println("Serial Over Flow!!!!!");
	  bleShield.flush();
	  myQueue.flush();
	}
    // シリアルからの読み込みが無いタイミングで処理する
    if (myQueue.size() > 0) {
	  read_data[buff_count] =  myQueue.pop();

	  if(buff_count == 1){
	    scan_len = (int)(read_data[buff_count]) + 3;
	  }
	  else if (buff_count == 4){
	    data->rssi = read_data[buff_count];
	  }
      // PacketType
	  else if (buff_count == 5){
	    data->packettype = read_data[buff_count];
	  }
      // Sender
	  else if ((5 < buff_count) && (buff_count < 12)){
	    data->sender[buff_count-6] = read_data[buff_count];
	  }
      // AddrType
	  else if (buff_count == 12){
	    data->addrtype = read_data[buff_count];
	  }
      // Bond
	  else if (buff_count == 13){
	    data->bond = read_data[buff_count];
	  }
      // data size
	  else if(buff_count == 14){
        data->data_len = (int)read_data[buff_count];
	  }
	  else if (buff_count > 14 ){
          data->data[data_count] = read_data[buff_count];
		  data_count++;
	  }

	  buff_count++;

	 // 終了判定
	  if (buff_count > scan_len){
        end_flg = true;
		break;
      }
	}
	time = millis();
	// データ取得件数が0、一定時間経過で検索を終了
	if ((buff_count == 0) || (time > timeoutEnd)){
		end_flg = true;
	}
  }
  // スキャンデータ判定
  if (scan_len < (int)0x0b){
	  buff_count = 0;
	  return false;
  }

  return true;

}

// Ble初期化
void ble113::init() {

  bleShield.begin(9600);
  Serial.begin(115200);
  
  // リセットコマンド
  bleShield.write((byte)0x00);
  bleShield.write((byte)0x01);
  bleShield.write((byte)0x00);
  bleShield.write((byte)0x00);
  bleShield.write((byte)0x00);

  delay(2000);
  // クリア
  bleShield.flush();
}

// set scan parameters
bool ble113::setScanParams(byte param[]) {

  bleShield.write((byte)0x00); // 0:コマンド
  bleShield.write((byte)0x05); // 1:サイズ　
  bleShield.write((byte)0x06); // class
  bleShield.write((byte)0x07);
  bleShield.write(param[0]); // scan_interval 1  0x00XX  0x4 - 0x0004
  bleShield.write(param[1]); // scan_interval 2  0xXX00
  bleShield.write(param[2]); // scan_window  1  0x00XX
  bleShield.write(param[3]); // scan_window  2  0xXX00
  bleShield.write(param[4]);  // 0x01,
  
  byte check_res[6];
  int i=0;
  // レスポンスチェック
  while (bleShield.available()){
    check_res[i]=bleShield.read();
    i++;
  }

  if(check_res[0]==0x00 &&
     check_res[1]==0x02 &&
     check_res[2]==0x06 &&
     check_res[3]==0x07 &&
     check_res[4]==0x00 &&
     check_res[4]==0x00) {
     return false; 
  }
  else {
    return true;
  }
}

// Scan開始
bool ble113::scanStart() {
  byte check_res[7];

  bleShield.write((byte)0x00); // 0:コマンド
  bleShield.write((byte)0x01); // 1:サイズ　
  bleShield.write((byte)0x06); // class
  bleShield.write((byte)0x02);
  bleShield.write((byte)0x01);

  for (int i = 0 ; i < 6 && bleShield.available() ; i++){
    check_res[i]=bleShield.read();
  }

  if(check_res[0]==0x00 &&
     check_res[1]==0x02 &&
     check_res[2]==0x06 &&
     check_res[3]==0x02 &&
     check_res[4]==0x00 &&
     check_res[5]==0x00) {
     return false; 
  }
  return true;
}

bool ble113::isRunning(){
    return isRunningFlag;
}

ble113 faboBLE;
