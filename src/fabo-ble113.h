/**
 @file fabo-ble113.h
 @brief This is a library for the FaBo BLE SiliconLabs Serial Brick.

   http://fabo.io/301.html

   Released under APACHE LICENSE, VERSION 2.0

   http://www.apache.org/licenses/

 @author FaBo<info@fabo.io>
*/

#ifndef BLE113_H
#define BLE113_H

#include "Arduino.h"
#include <SoftwareSerial.h>


/**
 @brief Scan data buffer
*/
#define BUFF_SIZE 11

/**
 * @class FaBoBLE
 * @brief FaBo BLE SiliconLabs Controll class
 */
class FaBoBLE
{
public:
    /**
      @struct ScanData
      @brief Scan Data structure
    */
    struct ScanData{
        int8_t rssi;
        byte packettype;
        byte sender[7];
        byte addrtype;
        byte bond;
        int data_len;
        byte data[50];
    };

    /**
       @struct BeaconParam
       @brief Beacon Data structure
    */
    struct BeaconParam {
        byte uuid[16];
        byte major[2];
        byte minor[2];
    };

    void setDebug(void);
    bool setAdvParameters(void);
    bool startAdv(void);
    void setBeaconUuid(byte uuid[]);
    void setBeaconMajor(byte major[]);
    void setBeaconMinor(byte minor[]);
    bool sendBeacon(void);
    bool stopAdv(void);
    bool isAdvertising(void);
    bool isScanning(void);

    void init();
    bool setScanParams(byte param[]);
    bool scan();
    void tick();

    bool getScanData(ScanData *out);
    int getDataCount(void);
    void hex_output(byte out_byte);

private:
    /**
      @brief Command(Set Adv Data).
      Set advertisement or scan response data.
      The custom data is only used when the discoverable mode is set to gap_user_data.
      V.1.3 API DOCUMENTATION Version 3.2 P95
    */
    byte COMMAND_SEND_BEACON[6] = { 0x00,   // message type, 0x00:command.
        0x20,     // Minimum payload length, 0x20=32 bytes.
        0x06,     // Message class, 0x06:Generic Access Profile.
        0x09,     // Message ID, 0x09.
        0x00,     // Advertisement data type, 0x00: sets advertisement data.
        0x1e};    // Advertisement data to send.

    /**
       @brief Command(Set Mode).
       Configures the current GAP discoverability and connectability mode.
       It is used to turn the module into a Slave by starting advertisement in connectable mode
       V.1.3 API DOCUMENTATION Version 3.2 P101
    */
    byte COMMAND_SET_MODE_FOR_BEACON[6] = {0x00,  // message type, 0x00:command
        0x02,     // Minimum payload length
        0x06,     // Message class, 0x06:Generic Access Profile
        0x01,     // Message ID, 0x01
        0x04,     // GAP Discoverable Mode
        0x00};    // GAP Connectable Mode

    /**
       @brief Command(Set Adv Parameters).
       Sets the advertising parameters.
       V.1.3 API DOCUMENTATION Version 3.2 P96.
    */
    byte COMMAND_SET_ADV_PARAMETERS_FOR_BEACON[9] = {0x00, // Message type -> 0x00:command.
        0x05, // Minimum payload length -> 0x05:5 Bytes.
        0x06, // Message class -> 0x06:Generic Access Profile.
        0x08, // Message ID -> 0x08.
        0x00, // Minimum advertisement[0].
        0x02, // Minimum advertisement[1].
        0x00, // Maximum advertisement[0].
        0x02, // Maximum advertisement[1].
        0x07}; //  Advertisement channels, 0x07: All three channels are used

    /**
       @brief Command(Set Mode).
       Configures the current GAP discoverability and connectability mode.
       It is used to turn the module into a Slave by starting advertisement in connectable mode
       V.1.3 API DOCUMENTATION Version 3.2 P101
    */
    byte COMMAND_STOP_MODE_STOP_FOR_BEACON[6] = {0x00,  // message type  -> 0x00:command.
        0x02,     // Minimum payload length -> 0x02:2 Bytes.
        0x06,     // Message class -> 0x06:Generic Access Profile.
        0x01,     // Message ID -> 0x01.
        0x00,     // GAP Discoverable Mode -> 0x00:gap_non_discoverable.
        0x00};    // GAP Connectable Mode.

    /**
       @brief Command(Discover).
       Resets the local device immediately.
       The command does not have a response.
       V.1.3 API DOCUMENTATION Version 3.2 P93
    */
    byte COMMAND_START_DISCOVER[5] = {0x00,  // message type  -> 0x00:command.
        0x01,     // Minimum payload length -> 0x01:1 Bytes.
        0x06,     // Message class -> 0x06:GAP.
        0x02,     // Message ID -> 0x02.
        0x01};    // mode -> 0x01:gap_discover_generic.

    /**
       @bfief Command(Reset).
       Resets the local device immediately.
       The command does not have a response.
       V.1.3 API DOCUMENTATION Version 3.2 P181
    */
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

    /**
       @brief count of buffer.
    */
    int buffCount = 0;

    /**
       @brief Read Buff
    */
    ScanData dataBuff[BUFF_SIZE];

    /**
       @brief flag of Debug.
    */
    bool DEBUG = false;

    /**
       @brief status of advertising.
    */
    bool is_advertising = false;

    /**
       @brief status of scanning.
    */
    bool is_scanning = false;

    /**
       @brief Wait time before BGAPI response.
    */
    int WAIT_REPLY = 1000;

    /**
       @brief Speed of BLE Brick.
    */
    int BLE_URAT = 9600;

    /**
       @brief Speed of Serial for DEBUG.
    */
    int DEBUG_URAT = 9600;
};

#endif
