#ifndef BLE_HANDLER_HPP
#define BLE_HANDLER_HPP

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID        "1a8e4e30-def4-4d96-a1f4-7e6101b7eea4"
#define CHARACTERISTIC_UUID  "8f9a9bc0-2bdd-40dd-8bb5-cb9d4e3799ff"

#define RUDDER         0
#define ELEVATOR       1
#define THROTTLE       2
#define AILERON        3
#define LOG            4
#define DPAD_UP        5
#define DPAD_DOWN      6
#define DPAD_LEFT      7
#define DPAD_RIGHT     8
#define BUTTON_ARM     9
#define BUTTON_FLIP    10
#define CONTROLMODE    11
#define ALTCONTROLMODE 12

#define RUDDER_MAX   511
#define RUDDER_MIN   -512
#define ELEVATOR_MAX 127
#define ELEVATOR_MIN -128
#define THROTTLE_MAX 511
#define THROTTLE_MIN -512
#define AILERON_MAX  127
#define AILERON_MIN  -128

#define LOG_MAX 1
#define LOG_MIN 0
#define CH6MAX  127
#define CH6MIN  -128

#define RUDDER_MAX_JOYC   100
#define ELEVATOR_MAX      127
#define THROTTLE_MAX_JOYC 100


extern volatile float Stick[16];

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
