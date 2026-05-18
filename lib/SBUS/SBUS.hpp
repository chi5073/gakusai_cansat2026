#pragma once
#include <Arduino.h>

class SBUSReceiver {
public:
    SBUSReceiver(HardwareSerial& serial = Serial1);
    void begin(int rxPin, int txPin);

    // 引数なしになり、非常にスッキリします
    void update();

    // スティックごとの値を取得する専用関数（直感的！）
    int getThrottle() const; // 0〜100%
    int getAileron() const;  // -100〜100%
    int getElevator() const; // -100〜100%
    int getRudder() const;   // -100〜100%

    uint16_t getChannel(uint8_t ch) const;
    bool isFailsafe() const;
    bool hasNewData() const;

private:
    HardwareSerial& _serial;
    uint16_t _channelData[16];
    
    // スティックデータをクラス内部に隠蔽
    int _stickData[4]; 

    bool _failsafe;
    bool _newData;

    void getData(uint8_t* data_received);
    void organizeData(const uint8_t* data_rcvd);
    void readSticks(); // 引数不要に変更
};