#include "motor_driver.hpp"

MotorDriver::MotorDriver(uint8_t pin1, uint8_t pin2, uint8_t channel, uint32_t pwmFreq, uint8_t pwmResolution)
    : _pin1(pin1), _pin2(pin2), _pwmChannel(channel), _pwmFreq(pwmFreq), _pwmResolution(pwmResolution) 
    {
    // 両方のピンを初期化
    pinMode(_pin1, OUTPUT);
    pinMode(_pin2, OUTPUT);

    // ESP32のPWM設定
    ledcSetup(_pwmChannel, _pwmFreq, _pwmResolution);
    stop(); 
}

void MotorDriver::command(uint8_t speed, bool direction) {
    uint8_t currentDir = direction;
    if (currentDir != _lastDirection) {
        if (direction) {
            ledcDetachPin(_pin2); 
            digitalWrite(_pin2, LOW);
            ledcAttachPin(_pin1, _pwmChannel);
        } else {
            ledcDetachPin(_pin1);
            digitalWrite(_pin1, LOW);
            ledcAttachPin(_pin2, _pwmChannel);
        }
        _lastDirection = currentDir;
    } 
    ledcWrite(_pwmChannel, speed);
}

void MotorDriver::stop() {
    ledcWrite(_pwmChannel, 0);
    ledcDetachPin(_pin1);
    ledcDetachPin(_pin2);
    digitalWrite(_pin1, LOW);
    digitalWrite(_pin2, LOW);
    _lastDirection = -1;
}
