#pragma once
#include <Arduino.h>
#include "motor_driver.hpp"
#include <cmath>

class RoverController { // 名前をRoverに変更
    public:
        RoverController(float steering_gain, float throttle_gain, MotorDriver* motor_right, MotorDriver* motor_left);
        void setup();
        // yawを符号付き整数(int16_t)、throttleも念のためint16_tに変更
        void command(int16_t yaw, int16_t throttle); 
    private:
        float _steering_gain;
        float _throttle_gain;
        MotorDriver* motor_right;
        MotorDriver* motor_left;
};