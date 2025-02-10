#ifndef BLE_HANDLER_H
#define BLE_HANDLER_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "stampfly.hpp"

#define SERVICE_UUID        "1a8e4e30-def4-4d96-a1f4-7e6101b7eea4"
#define CHARACTERISTIC_UUID  "8f9a9bc0-2bdd-40dd-8bb5-cb9d4e3799ff"

class BLEHandler {
public:
    BLEHandler();
    void initBLE();
    void sendSensorData();
    bool isDeviceConnected();
    
private:
    BLEServer* pServer;
    BLECharacteristic* pCharacteristic;
    bool deviceConnected;
    
    // Callback classes require reference to BLEHandler instance
    class MyServerCallbacks : public BLEServerCallbacks {
    public:
        MyServerCallbacks(BLEHandler& handler) : handler(handler) {}
        void onConnect(BLEServer* pServer) override;
        void onDisconnect(BLEServer* pServer) override;
    private:
        BLEHandler& handler;
    };

    class MyCallbacks : public BLECharacteristicCallbacks {
    public:
        MyCallbacks(BLEHandler& handler) : handler(handler) {}
        void onWrite(BLECharacteristic* pCharacteristic) override;
    private:
        BLEHandler& handler;
    };

    friend class MyServerCallbacks;
    friend class MyCallbacks;
};

#endif // BLE_HANDLER_H
