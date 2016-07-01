/**
 @file fabo-ble113.cpp
 @brief This is a library for the FaBo BLE SiliconLabs Serial Brick.

   http://fabo.io/301.html

   Released under APACHE LICENSE, VERSION 2.0

   http://www.apache.org/licenses/

 @author FaBo<info@fabo.io>
*/

#include "fabo-ble113.h"
#include <SoftwareSerial.h>

FaBoBLE::BeaconParam beaconParam;

//! Serial connection for BLE113 module
SoftwareSerial bleBrick(12, 13);

/**
   @brief Enable debug message.
*/
void FaBoBLE::setDebug(){
    DEBUG = true;
}

/**
 * @brief Set beacon uuid.
 * @param [in] uuid[] UUID(16bytes)
 */
void FaBoBLE::setBeaconUuid(byte uuid[]){

    memcpy(beaconParam.uuid, uuid, 16);
}

/**
   @brief Set beacon major id.
   @param [in] major[] majorId(2bytes)
*/
void FaBoBLE::setBeaconMajor(byte major[]){

    memcpy(beaconParam.major, major, 2);
}

/**
  @brief Set beacon minor id.
  @param [in] minor[] minorId(2bytes)
*/
void FaBoBLE::setBeaconMinor(byte minor[]){

    memcpy(beaconParam.minor, minor, 2);
}

/**
   @brief send beacon(start advertising).
*/
bool FaBoBLE::sendBeacon(){

    // send BGAPI command.
    sendCommand(COMMAND_SEND_BEACON, 6);

    /**
       @brief iBeacon header.
    */
    byte beaconHeader[9] = {  0x02, // Flags[0], Bluetooth 4.0 Core Specification
        0x01, // Flags[1], Bluetooth 4.0 Core Specification
        0x06, // Flags[2], Bluetooth 4.0 Core Specification
        0x1A, // Length, Bluetooth 4.0 Core Specification
        0xFF, // Type, Bluetooth 4.0 Core Specification
        0x4C, // Company ID[0]
        0x00, // Company ID[1]
        0x02, // Beacon Type[0]
        0x15}; // Beacon Type[1]

    // txPower
    byte txPower = 0xC9;

    // send iBeacon header.
    for(int i = 0; i < 9; i++){
        bleBrick.write((byte)beaconHeader[i]);
    }

    // send iBeacon UUID.
    for(int i = 0; i < 16; i++){
        bleBrick.write((byte)beaconParam.uuid[i]);
    }

    // send iBeacon MajorID.
    for(int i = 0; i < 2; i++){
        bleBrick.write((byte)beaconParam.major[i]);
    }

    // send iBeacon MinorID.
    for(int i = 0; i < 2; i++){
        bleBrick.write((byte)beaconParam.minor[i]);
    }

    // send txPower.
    bleBrick.write(txPower);

    // Waiting reply.
    delay(WAIT_REPLY);

    // Receive reply.
    byte buffer[10];
    int i = 0;
    while (bleBrick.available()) {
        buffer[i] = bleBrick.read();
        i++;
    }

    if(errorCheck(buffer)){
        is_advertising = true;
        return true;
    } else {
        return false;
    }
}

/**
   @brief start advertising.
*/
bool FaBoBLE::startAdv()
{
    // send BGAPI command.
    sendCommand(COMMAND_SET_MODE_FOR_BEACON, 6);

    // Waiting reply.
    delay(WAIT_REPLY);

    // recieve BGAPI response.
    byte buffer[10];
    int i = 0;
    while (bleBrick.available()) {
        buffer[i] = bleBrick.read();
        i++;
    }

    return errorCheck(buffer);
}

/**
   @brief Set Adv Parameters.
*/
bool FaBoBLE::setAdvParameters()
{

    // send BGAPI command.
    sendCommand(COMMAND_SET_ADV_PARAMETERS_FOR_BEACON, 9);

    // Waiting reply.
    delay(WAIT_REPLY);

    // Receive BGAPI response.
    byte buffer[10];
    int i = 0;
    while (bleBrick.available()) {
        buffer[i] = bleBrick.read();
        i++;
    }

    return errorCheck(buffer);
}

/**
 * stop advertising.
 */
bool FaBoBLE::stopAdv()
{

    // send BGAPI command.
    sendCommand(COMMAND_STOP_MODE_STOP_FOR_BEACON, 6);

    // Waiting reply.
    delay(WAIT_REPLY);

    // Receive BGAPI response.
    byte buffer[10];
    int i = 0;
    while (bleBrick.available()) {
        buffer[i] = bleBrick.read();
        i++;
    }

    if(errorCheck(buffer)){
        is_advertising = false;
        return true;
    } else {
        return false;
    }
}

/**
 * scan data analysis.
 */
void FaBoBLE::tick() {

    uint8_t readData;
    if(!broken){
        if(bleBrick.overflow()){
            if(DEBUG){
                Serial.println("Serial Over Flow!!!!!");
            }
            broken = true;
            pos = 0;
        }
    }

    if (bleBrick.available()) {
        readData = bleBrick.read();
        // skip broken data
        if (broken){
            return;
        }

        ScanData &data = dataBuff[dataIn];

        switch(pos){
            case 0:
                if (readData != 0x80) {
                    broken = true;
                }
                break;

            case 1:
                scanLen = (int)(readData) + 3;
                // impossible value is broken data...
                if (scanLen < 0 || scanLen > 50) {
                    broken = true;
                  }
                break;

            case 4:
                data.rssi = readData;
                break;

            // PacketType
            case 5:
                data.packettype = readData;
                break;

            // AddrType
            case 12:
                data.addrtype = readData;
                break;

            // Bond
            case 13:
                data.bond = readData;
                break;

            // data size
            case 14:
                data.data_len = (int)readData;
                break;

            // Sender , data
            default:
                // Sender
                if ((5 < pos) && (pos < 12)){
                    data.sender[pos-6] = readData;
                }
                else if (pos > 14 ){
                    data.data[pos - 14] = readData;
                }
                break;
        }

        pos++;

        // end of data
        if (pos > scanLen){
            pos = 0;
            scanLen = 1;
            dataCount++;
            dataIn++;
            dataIn %= BUFF_SIZE;
            if (dataIn == dataOut) {
              //Delete old data
              dataOut++;
              dataCount--;
            }
        }
    }
    else {
        pos = 0;
        scanLen = 1;
        broken = false;
    }
}

/**
 * @brief BLE init.
 */
void FaBoBLE::init() {
    // Connect bleBrick.
    bleBrick.begin(BLE_URAT);

    // send BGAPI command.
    sendCommand(COMMAND_RESET, 5);

    // Wait reply.
    delay(WAIT_REPLY);

    // Flash of software serial.
    byte buffer[1];
    while (bleBrick.available()) {
        buffer[0] = bleBrick.read();
    }
}

/**
  @brief Set scan parameters
  @param [in] param[] scan parameters
*/
bool FaBoBLE::setScanParams(byte param[]) {
    bleBrick.flush();

    bleBrick.write((byte)0x00);
    bleBrick.write((byte)0x05);
    bleBrick.write((byte)0x06); // class
    bleBrick.write((byte)0x07);
    bleBrick.write(param[0]); // scan_interval 1  0x00XX  0x4 - 0x0004
    bleBrick.write(param[1]); // scan_interval 2  0xXX00
    bleBrick.write(param[2]); // scan_window  1  0x00XX
    bleBrick.write(param[3]); // scan_window  2  0xXX00
    bleBrick.write(param[4]);  // 0x01,

    // Waiting reply.
    delay(WAIT_REPLY);

    // Receive BGAPI response.
    byte buffer[10];
    int i = 0;
    while (bleBrick.available()) {
        buffer[i] = bleBrick.read();
        i++;
    }

    return errorCheck(buffer);
}

/**
   @brief BLE Scan start.
*/
bool FaBoBLE::scan() {

    // send BGAPI command.
    sendCommand(COMMAND_START_DISCOVER,5);

    // Waiting reply.
    delay(100);

    // Receive BGAPI response.
    byte buffer[10];
    int i = 0;
    while (bleBrick.available()) {
        buffer[i] = bleBrick.read();

        i++;
        if (i > 5){
          break;
        }
    }

    return errorCheck(buffer);
}

/**
   @brief Status of advertising.
   @retval true  is advertising
   @retval false is not advertising
*/
bool FaBoBLE::isAdvertising(){
    return is_advertising;
}

/**
   @brief Status is scanning.
   @retval true  is scanning
   @retval false is not scanning
*/
bool FaBoBLE::isScanning(){
    return is_scanning;
}

/**
  @brief Send command.
  @param [in] command[] command bytes
  @param [in] length    command length
*/
void FaBoBLE::sendCommand(byte command[], int length)
{

    // send BGAPI command.
    for(int i = 0; i < length; i++){
        bleBrick.write((byte)command[i]);
    }
}

/**
   @brief Error checking of the response data.
   @param [in] buffer response data
*/
bool FaBoBLE::errorCheck(byte buffer[]){
    // Error check.
    if(buffer[0] == 0x00 && buffer[1] == 0x02 && buffer[2] == 0x06){
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

/**
 * @fn
 * Get ScanData Count.
 */
int FaBoBLE::getDataCount() {
	  return dataCount;
}

/**
 * @fn
 * Get ScanData.
 */
bool FaBoBLE::getScanData(ScanData *out) {
	  if (dataCount == 0) return false;
	  memcpy(out, &dataBuff[dataOut++], sizeof(ScanData));
	  dataOut %= BUFF_SIZE;
	  dataCount--;
	  return true;
}
