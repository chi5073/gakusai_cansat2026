#pragma once
#include <Arduino.h>

class MotorDriver {
public:
    MotorDriver(uint8_t pin1, uint8_t pin2, uint8_t channel, uint32_t pwmFreq = 3000, uint8_t pwmResolution = 8);
    void command(uint8_t speed, bool direction = true); 
    void stop();

private:
    uint8_t _pin1;
    uint8_t _pin2;
    uint8_t _pwmChannel;
    uint32_t _pwmFreq;
    uint8_t _pwmResolution;
    uint8_t _lastDirection = -1; // -1: 停止, 1: 正転, 0: 逆転
};

