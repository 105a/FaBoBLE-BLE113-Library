/**
 * @file fabo-ble113.cpp
 * @brief fabo libtary of BLE113 BGAPI
 * @author Akira Sasaki, Shimakage Kouji
 * @date 2,6, 2015
 */

#include "fabo-ble113.h"
#include <SoftwareSerial.h>

FaboQueue myQueue;
FaBoBLE::BeaconParam beaconParam;

//! Serial connection for BLE113 module
SoftwareSerial bleBrick(12, 13);

/**
 * @fn
 * Enable debug message.
 */
void FaBoBLE::setDebug(){
    DEBUG = true;
}

/**
 * @fn
 * Set beacon uuid.
 * @param uuid[] UUID(16bytes)
 */
void FaBoBLE::setBeaconUuid(byte uuid[]){
    
    memcpy(beaconParam.uuid, uuid, 16);
}

/**
 * @fn
 * Set beacon major id.
 * @param major[] majorId(2bytes)
 */
void FaBoBLE::setBeaconMajor(byte major[]){
    
    memcpy(beaconParam.major, major, 2);
}

/**
 * @fn
 * Set beacon minor id.
 * @param major[] minorId(2bytes)
 */
void FaBoBLE::setBeaconMinor(byte minor[]){
    
    memcpy(beaconParam.minor, minor, 2);
}

/**
 * @fn
 * send beacon(start advertising).
 */
bool FaBoBLE::sendBeacon(){
    
    // send BGAPI command.
    sendCommand(COMMAND_SEND_BEACON, 6);
    
    // iBeacon header.
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
 * @fn
 * set mode.
 */
bool FaBoBLE::setMode()
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
 * @fn
 * Set Adv Parameters.
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
 * @fn
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
 * @fn
 * scan.
 */
bool FaBoBLE::scan(ScanData *data) {
    int i = 0;
    int timeout = 100;
    
    long time = millis();
    long timeoutEnd = time + timeout;
    buff_count =0;
    uint8_t read_data[255];
    int len;
    int data_count =0;
    bool end_flg = false;
    int scan_len = 4;
    
    while(!end_flg){
        if(bleBrick.overflow()){
            if(DEBUG){
                Serial.println("Serial Over Flow!!!!!");
            }
            bleBrick.flush();
            myQueue.flush();
        }
        
        // Push data to FIFO from buffer.
        while (bleBrick.available()) {
            myQueue.push(bleBrick.read());
        }
        
        if((myQueue.size() >= 255)){
            if(DEBUG){
                Serial.println("Serial Over Flow!!!!!");
            }
            bleBrick.flush();
            myQueue.flush();
        }
        
        // �V���A������̓ǂݍ��݂������^�C�~���O�ŏ�������
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
            
            // �I������
            if (buff_count > scan_len){
                end_flg = true;
                break;
            }
        }
        time = millis();
        // �f�[�^�擾������0�A��莞�Ԍo�߂Ō������I��
        if ((buff_count == 0) || (time > timeoutEnd)){
            end_flg = true;
        }
    }
    // �X�L�����f�[�^����
    if (scan_len < (int)0x0b){
        buff_count = 0;
        return false;
    }
    
    return true;
    
}

/**
 * @fn
 * init.
 */
void FaBoBLE::init() {

    // Connect bleBrick.
    bleBrick.begin(BLE_URAT);
    
    // send BGAPI command.
    sendCommand(COMMAND_RESET, 5);
    
    // Wait reply.
    delay(2000);
    
    // Flash of software serial.
    bleBrick.flush();
}

/**
 * @fn
 * Set scan parameters
 *
 */
bool FaBoBLE::setScanParams(byte param[]) {
    
    
    bleBrick.write((byte)0x00); // 0:�R�}���h
    bleBrick.write((byte)0x05); // 1:�T�C�Y�@
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
 * @fn
 * Scan start.
 */
bool FaBoBLE::scanStart() {
    
    // send BGAPI command.
    sendCommand(COMMAND_START_DISCOVER,5);
    
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
 * @fn
 * Status of advertising.
 */
bool FaBoBLE::isAdvertising(){
    return is_advertising;
}

/**
 * @fn
 * Status is scanning.
 */
bool FaBoBLE::isScanning(){
    return is_scanning;
}

/**
 * @fn
 * Send command.
 */
void FaBoBLE::sendCommand(byte command[], int length)
{

    // send BGAPI command.
    for(int i = 0; i < length; i++){
        bleBrick.write((byte)command[i]);
    }
}


/**
 * @fn
 * Error check.
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

