#include <SoftwareSerial.h>
#include <ble113.h>

ble113  myBle113;
typeScanData buff;
static typeScanData buff_init;

void setup() {
  // BLEとの通信用
  //  bleShield.begin(9600);
  // ログ出力用
  Serial.begin(115200);
  Serial.write("*Start!\n");

  Serial.println("sd_ble_init()");
  myBle113.init();
  delay(1000);

  Serial.println("");

  // Scanパラメータセット
  Serial.write("sd_ble_enable()\n");
  byte param[5] = {0x80,  //
                   0x00,  //
                   0x0A,  //
                   0x00,  //
                   0x01
                  }; //
  if (myBle113.setScanParams(param)) {
    Serial.println("param set OK!");
  }
  else {
    Serial.println("param set NG!");
  }

  Serial.println("");

  Serial.println("sd_ble_gap_scan_start()");
  if (myBle113.scanStart()) {
    Serial.println("Scan OK!");
  }
  else {
    Serial.println("Scan NG!");
  }
}

void loop() {

  if (myBle113.scan(&buff)){
    Serial.print("RSSI:");
    Serial.print(buff.rssi);
    Serial.print(" PacketType:");
    hex_output(buff.packettype);
    Serial.print(" Sender:");
    hex_output(buff.sender[0]);
    hex_output(buff.sender[1]);
    hex_output(buff.sender[2]);
    hex_output(buff.sender[3]);
    hex_output(buff.sender[4]);
    hex_output(buff.sender[5]);
    Serial.print(" AddrType:");
    hex_output(buff.addrtype);
    Serial.print(" Bond:");
    hex_output(buff.bond);
//    Serial.print(" DataSize:");
//    Serial.print(buff.data_len);
    Serial.print(" Data:");
    for (int i = 0 ; i < buff.data_len ; i++) {
      hex_output(buff.data[i]);
    }
    Serial.println("");
  }

  // バッファの初期化
  buff = buff_init;

  delay(20);
}

// 出力処理(16進数２桁)
void hex_output(byte out_byte) {
  if (out_byte < 0x10) {
    Serial.print("0");
  }
  Serial.print(out_byte, HEX);
}

