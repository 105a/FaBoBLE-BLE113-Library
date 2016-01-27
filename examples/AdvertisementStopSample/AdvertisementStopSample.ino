#include <SoftwareSerial.h>

SoftwareSerial bleShield(12, 13);

#define buttonPin A0 // ボタンピン
#define ledPin A1    // LEDピン

// ボタンの押下状況取得用
int buttonState = 0;


void setup() {

  // ボタンピンを入力用に設定
  pinMode(buttonPin, INPUT); 
  // LEDピンを出力用に設定
  pinMode(ledPin, OUTPUT);    
  
  // BLEとの通信用
  bleShield.begin(9600);
  // ログ出力用
  Serial.begin(9600);
  Serial.write("Adv start!");

  // Set Adv Parameter
  
  bleShield.print(0x00,HEX);
  bleShield.print(0x05,HEX);
  bleShield.print(0x06,HEX);
  bleShield.print(0x08,HEX);
  bleShield.print(0x00,HEX);
  bleShield.print(0x02,HEX);
  bleShield.print(0x00,HEX);
  bleShield.print(0x02,HEX);
  bleShield.print(0x07,HEX);
}

void loop() {

    // ボタンの押下状況を取得
  buttonState = digitalRead(buttonPin);

  // ボタン押下判定
  if (buttonState == HIGH) {        
    // LED点灯
    digitalWrite(ledPin, HIGH);
    stopAdv();  
  } 
  else {
    // LED消灯
    digitalWrite(ledPin, LOW); 
    startAdv();
  }

}

void stopAdv(){
  //SetMode_AdvStop
    bleShield.write((byte)0x00);  // message type  ->0x00:command
    bleShield.write((byte)0x02);  // Minimum payload length
    bleShield.write((byte)0x06);  // Message class -> 0x06:Generic Access Profile
    bleShield.write((byte)0x01);  // Message ID
    bleShield.write((byte)0x00);  // GAP Discoverable Mode gap_non_discoverable
    bleShield.write((byte)0x00);  // GAP Connectable Mode gap_discover_limited
}

void startAdv(){
    Serial.println("*");
    bleShield.write((byte)0x00);  // message type  ->0x00:command
    bleShield.write((byte)0x02);  // Minimum payload length
    bleShield.write((byte)0x06);  // Message class -> 0x06:Generic Access Profile
    bleShield.write((byte)0x01);  // Message ID
    bleShield.write((byte)0x04);
    bleShield.write((byte)0x00);
    delay(1000);
    byte value[6];
    int i = 0;
    while (bleShield.available()) {
      if(i<6){
        value[i] = bleShield.read();

          if( i == 5){
              if(value[0] == 0x00 && 
                 value[1] == 0x02 &&
                 value[2] == 0x06 &&
                 value[3] == 0x01 &&
                 value[4] == 0x00 && 
                 value[5] == 0x00)
            {
             Serial.println("Command OK[1]");
             break;
            }
          }
        i++;
      }else if(i>5){
          break;
        }
    }
    Serial.print("command1:");
    for (int j=0 ; j<6 ; j++){
        Serial.print(value[j]);
        Serial.print(",");
    }

    Serial.println("");
    delay(500);

    bleShield.write((byte)0x00); // 2 :message type  ->0x00:command
    bleShield.write((byte)0x20); // 3 :Minimum payload length
    bleShield.write((byte)0x06); // 4 :Message class -> 0x06:Generic Access Profile
    bleShield.write((byte)0x09); // 5 :Message ID
    bleShield.write((byte)0x00); // 6 :Advertisement data type -> 0x00: sets advertisement data 
    bleShield.write((byte)0x1e); // 7-:Advertisement data to send

    bleShield.write((byte)0x02); //
    bleShield.write((byte)0x01);
    bleShield.write((byte)0x06);
    bleShield.write((byte)0x1A);
    bleShield.write((byte)0xFF);
    bleShield.write((byte)0x4C);
    bleShield.write((byte)0x00);
    bleShield.write((byte)0x02);
    bleShield.write((byte)0x15);
    bleShield.write((byte)0xCB);
    bleShield.write((byte)0x86);
    bleShield.write((byte)0xBC);
    bleShield.write((byte)0x31);
    bleShield.write((byte)0x05);
    bleShield.write((byte)0xBD);
    bleShield.write((byte)0x40);
    bleShield.write((byte)0xCC);
    bleShield.write((byte)0x90);
    bleShield.write((byte)0x3D);
    bleShield.write((byte)0x1C);
    bleShield.write((byte)0x9B);
    bleShield.write((byte)0xD1);
    bleShield.write((byte)0x3D);
    bleShield.write((byte)0x96);
    bleShield.write((byte)0x6B);
    bleShield.write((byte)0x00);
    bleShield.write((byte)0x02);
    bleShield.write((byte)0x00);
    bleShield.write((byte)0x01);
    bleShield.write((byte)0xC9);

    delay(1000);

    i = 0;
    while (bleShield.available()) {
      value[i] = bleShield.read();
      if(i<6){

        if( i == 5){
          if(value[0] == 0x00 && 
             value[1] == 0x02 &&
             value[2] == 0x06 &&
             value[3] == 0x09 &&
             value[4] == 0x00 && 
             value[5] == 0x00)
            {
                Serial.println("Command OK[2]");
                break;
            }
        }

        i++;
      }
    }
    Serial.print("command2:");
    for (int k=0 ; k<6 ; k++){
        Serial.print(value[k]);
        Serial.print(",");
    }
    Serial.println("");

  }
