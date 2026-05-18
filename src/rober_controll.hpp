#pragma once
#include <Arduino.h>
#include "motor_driver.hpp"

class RoberController{
    public:
        RoberController(float steering_gain, float throttle_gain, MotorDriver* motor_right, MotorDriver* motor_left);
        void setup();
        void command(uint16_t yaw, uint16_t throttle); // yaw: 0-360, throttle: 0-2047
    private:
        float _steering_gain; // 0-1の範囲で旋回を調整するためのゲイン
        float _throttle_gain; // 0-1の範囲でスロットルを調整するためのゲイン
        MotorDriver* motor_right;
        MotorDriver* motor_left;
};
