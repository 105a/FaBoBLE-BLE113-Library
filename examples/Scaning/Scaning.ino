#include <SoftwareSerial.h>
#include <fabo-ble113.h>

FaBoBLE  myBle;
FaBoBLE::ScanData buff;

void setup() {
  // ログ出力用
  Serial.begin(115200);
  Serial.println("Start!");
  myBle.setDebug();

  Serial.println("sd_ble_init()");
  myBle.init();

  Serial.println("");

  // Scanパラメータセット
  Serial.write("sd_ble_enable()\n");
  byte param[5] = {0x80,  // interval1 データ検索間隔 0x4 - 0x4000
                   0x00,  // interval2  1 -> 625us
                   0x08,  // window1   検索時間 0x4 - 0x4000
                   0x00,  // window2    1 -> 625us
                   0x01   // 1:Active scanning, 0:Passive scanning
                  }; //
  if (myBle.setScanParams(param)) {
    Serial.println("param set OK!");
  }
  else {
    Serial.println("param set NG!");
  }

  Serial.println("");
  Serial.println("sd_ble_gap_scan_start()");
  if (myBle.scan()) {
    Serial.println("Scan OK!");
  }
  else {
    Serial.println("Scan NG!");
  }
}

void loop() {
  // BLE内部処理のためloop内で呼び出してください
  myBle.tick();

  // レコードが存在する場合出力
  while (myBle.getScanData(&buff)) {
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
    Serial.print(" Data:");
    for (int i = 0 ; i < buff.data_len ; i++) {
      hex_output(buff.data[i]);
    }
    Serial.println("");
  }

  delay(2);
}

// 出力処理(16進数２桁)
void hex_output(byte out_byte) {
  if (out_byte < 0x10) {
    Serial.print("0");
  }
  Serial.print(out_byte, HEX);
}

