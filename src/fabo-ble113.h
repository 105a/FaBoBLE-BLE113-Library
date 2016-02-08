#ifndef BLE113_H
#define BLE113_H

#include "Arduino.h"
#include <SoftwareSerial.h>

#define BUFF_SIZE 11

class FaBoBLE

{
public:

    //! Scan data struct
    struct ScanData{
        int8_t rssi;
        byte packettype;
        byte sender[7];
        byte addrtype;
        byte bond;
        int data_len;
        byte data[50];
    };

    //! Beacon data struct
    struct BeaconParam {
        byte uuid[16];
        byte major[2];
        byte minor[2];
    };


    void setDebug(void);
    bool setAdvParameters(void);
    bool setMode(void);
    void setBeaconUuid(byte uuid[]);
    void setBeaconMajor(byte major[]);
    void setBeaconMinor(byte minor[]);
    bool sendBeacon(void);
    bool stopAdv(void);
    bool isAdvertising(void);
    bool isScanning(void);

    // Ble初期化
    void init();
    // set scan parameters
    bool setScanParams(byte param[]);
    // Scan開始
    bool scan();
    // データ解析
    void tick();

    bool getScanData(ScanData *out);
    int getDataCount(void);
    // 出力処理(16進数２桁)
    void hex_output(byte out_byte);

//    // 取得イベント
//    void (*ble_evt_scan_res)(const struct ScanData *data);


private:
    // Command(Set Adv Data).
    // Set advertisement or scan response data.
    // The custom data is only used when the discoverable mode is set to gap_user_data.
    // V.1.3 API DOCUMENTATION Version 3.2 P95
    byte COMMAND_SEND_BEACON[6] = { 0x00,   // message type, 0x00:command.
        0x20,     // Minimum payload length, 0x20=32 bytes.
        0x06,     // Message class, 0x06:Generic Access Profile.
        0x09,     // Message ID, 0x09.
        0x00,     // Advertisement data type, 0x00: sets advertisement data.
        0x1e};    // Advertisement data to send.

    // Command(Set Mode).
    // Configures the current GAP discoverability and connectability mode.
    // It is used to turn the module into a Slave by starting advertisement in connectable mode
    // V.1.3 API DOCUMENTATION Version 3.2 P101
    byte COMMAND_SET_MODE_FOR_BEACON[6] = {0x00,  // message type, 0x00:command
        0x02,     // Minimum payload length
        0x06,     // Message class, 0x06:Generic Access Profile
        0x01,     // Message ID, 0x01
        0x04,     // GAP Discoverable Mode
        0x00};    // GAP Connectable Mode

    // Command(Set Adv Parameters).
    // Sets the advertising parameters.
    // V.1.3 API DOCUMENTATION Version 3.2 P96.
    byte COMMAND_SET_ADV_PARAMETERS_FOR_BEACON[9] = {0x00, // Message type -> 0x00:command.
        0x05, // Minimum payload length -> 0x05:5 Bytes.
        0x06, // Message class -> 0x06:Generic Access Profile.
        0x08, // Message ID -> 0x08.
        0x00, // Minimum advertisement[0].
        0x02, // Minimum advertisement[1].
        0x00, // Maximum advertisement[0].
        0x02, // Maximum advertisement[1].
        0x07}; //  Advertisement channels, 0x07: All three channels are used

    // Command(Set Mode).
    // Configures the current GAP discoverability and connectability mode.
    // It is used to turn the module into a Slave by starting advertisement in connectable mode
    // V.1.3 API DOCUMENTATION Version 3.2 P101
    byte COMMAND_STOP_MODE_STOP_FOR_BEACON[6] = {0x00,  // message type  -> 0x00:command.
        0x02,     // Minimum payload length -> 0x02:2 Bytes.
        0x06,     // Message class -> 0x06:Generic Access Profile.
        0x01,     // Message ID -> 0x01.
        0x00,     // GAP Discoverable Mode -> 0x00:gap_non_discoverable.
        0x00};    // GAP Connectable Mode.

    // Command(Discover).
    // Resets the local device immediately.
    // The command does not have a response.
    // V.1.3 API DOCUMENTATION Version 3.2 P93
    byte COMMAND_START_DISCOVER[5] = {0x00,  // message type  -> 0x00:command.
        0x01,     // Minimum payload length -> 0x01:1 Bytes.
        0x06,     // Message class -> 0x06:GAP.
        0x02,     // Message ID -> 0x02.
        0x01};    // mode -> 0x01:gap_discover_generic.

    // Command(Reset).
    // Resets the local device immediately.
    // The command does not have a response.
    // V.1.3 API DOCUMENTATION Version 3.2 P181
    byte COMMAND_RESET[5] = {0x00,  // message type  -> 0x00:command.
        0x01,     // Minimum payload length -> 0x01:1 Bytes.
        0x00,     // Message class -> 0x00:System.
        0x00,     // Message ID -> 0x01.
        0x00};    // Selects the boot mode -> 0x00:boot to main program

    bool errorCheck(byte buffer[]);

    void sendCommand(byte command[], int length);

    int pos = 0;
    int dataCount = 0;
    int scanLen = 1;
    int dataIn = 0;
    int dataOut = 0;
    bool broken = false;

    //! count of buffer.
    int buffCount = 0;

    // Read Buff
    ScanData dataBuff[BUFF_SIZE];

    //! flag of Debug.
    bool DEBUG = false;

    //! status of advertising.
    bool is_advertising = false;

    //! status of scanning.
    bool is_scanning = false;

    // Wait time before BGAPI response.
    int WAIT_REPLY = 1000;

    // Speed of BLE Brick.
    int BLE_URAT = 9600;

    // Speed of Serial for DEBUG.
    int DEBUG_URAT = 115200;
};

#endif
