#include "BLEHandler.h"
#include "sensor.hpp"
#include "flight_control.hpp"

BLEHandler::BLEHandler() : pServer(nullptr), pCharacteristic(nullptr), deviceConnected(false) {}

void BLEHandler::MyServerCallbacks::onConnect(BLEServer* pServer) {
    USBSerial.printf("BLE Connedted!\r\n");
    handler.deviceConnected = true;
    delay(2000);
    USBSerial.printf("BLE Ready!\r\n");
}

void BLEHandler::MyServerCallbacks::onDisconnect(BLEServer* pServer) {
    USBSerial.printf("BLE Disconnedted!\r\n");
    handler.deviceConnected = false;
    BLEDevice::startAdvertising();
}

void BLEHandler::MyCallbacks::onWrite(BLECharacteristic* pCharacteristic) {
    std::string receivedData = pCharacteristic->getValue();
    if (receivedData.length() > 0) {
        USBSerial.printf("BLE Data Received!\r\n");
        float receivedValue;
        memcpy(&receivedValue, receivedData.data(), sizeof(float));
        // Process received data here if needed
    }
}

void BLEHandler::initBLE() {
    BLEDevice::init("StampFly_BLE");
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks(*this));  // Pass BLEHandler instance

    BLEService* pService = pServer->createService(SERVICE_UUID);

    pCharacteristic = pService->createCharacteristic(
                            CHARACTERISTIC_UUID,
                            BLECharacteristic::PROPERTY_READ |
                            BLECharacteristic::PROPERTY_WRITE |
                            BLECharacteristic::PROPERTY_INDICATE
                          );

    pCharacteristic->setCallbacks(new MyCallbacks(*this));  // Pass BLEHandler instance

    pService->start();

    BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    BLEDevice::startAdvertising();
}

void BLEHandler::sendSensorData() {
    if (deviceConnected) {
        // Send the sensor data over BLE
        uint8_t sensorData[44]; // 11 floats * 4 bytes = 48 bytes
        float data;
        data = Elapsed_time;
        memcpy(sensorData, &data, sizeof(float));
        data = Interval_time;
        memcpy(sensorData + 4, &data, sizeof(float));
        data =  (Roll_angle - Roll_angle_offset) * 180 / PI;
        memcpy(sensorData + 8, &data, sizeof(float));
        data = (Pitch_angle - Pitch_angle_offset) * 180 / PI;
        memcpy(sensorData + 12, &data, sizeof(float));
        data = (Yaw_angle - Yaw_angle_offset) * 180 / PI;
        memcpy(sensorData + 16, &data, sizeof(float));
        data = Roll_rate * 180 / PI;
        memcpy(sensorData + 20, &data, sizeof(float));
        data = Pitch_rate * 180 / PI;
        memcpy(sensorData + 24, &data, sizeof(float));
        data = Yaw_rate * 180 / PI;
        memcpy(sensorData + 28, &data, sizeof(float));
        data = Accel_x_raw;
        memcpy(sensorData + 32, &data, sizeof(float));
        data = Accel_y_raw;
        memcpy(sensorData + 36, &data, sizeof(float));
        data = Accel_z_raw;
        memcpy(sensorData + 40, &data, sizeof(float));
        data = Alt_velocity;
        memcpy(sensorData + 44, &data, sizeof(float));
        data = Z_dot_ref;
        memcpy(sensorData + 48, &data, sizeof(float));
        data = Alt_ref;
        memcpy(sensorData + 52, &data, sizeof(float));
        data = Altitude2;
        memcpy(sensorData + 56, &data, sizeof(float));        
        data = Altitude;
        memcpy(sensorData + 60, &data, sizeof(float));        
        data = Az;
        memcpy(sensorData + 64, &data, sizeof(float));
        data = Az_bias;
        memcpy(sensorData + 68, &data, sizeof(float));
        data = Alt_flag;
        memcpy(sensorData + 72, &data, sizeof(float));
        data = Mode;
        memcpy(sensorData + 76, &data, sizeof(float));
        data = RangeFront;
        memcpy(sensorData + 80, &data, sizeof(float));

        pCharacteristic->setValue(sensorData, sizeof(sensorData));
        //pCharacteristic->notify();
        //USBSerial.printf("BLE Sending %d Bytes!\r\n", sizeof(sensorData));
    }
}

bool BLEHandler::isDeviceConnected() {
    return deviceConnected;
}
