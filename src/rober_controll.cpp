#include "rober_controll.hpp"

RoberController::RoberController(float steering_gain, float throttle_gain, MotorDriver* motor_right, MotorDriver* motor_left){
    this->_steering_gain = steering_gain;
    this->_throttle_gain = throttle_gain;
    this->motor_right = motor_right;
    this->motor_left = motor_left;
    this->motor_right->command(0); 
    this->motor_left->command(0); 
}

void RoberController::setup(){
    // ここでは特に初期化は必要ないが、将来的にセンサーのキャリブレーションなどを行う場合はここに記述する
}

void RoberController::command(uint16_t yaw, uint16_t throttle){
    // yawとthrottleはそれぞれ0-360、0-2047の範囲で受け取る想定
    // これらをモーターの速度と方向に変換する

    // モーターの速度と方向を計算
    int8_t right_speed = map(this->_throttle_gain*throttle + this->_steering_gain*(yaw), -100, 300, 0, 255); // 右モーターはヨーの影響で加速
    int8_t left_speed = map(this->_throttle_gain*(throttle) - this->_steering_gain*(yaw), -100, 300, 0, 255);  // 左モーターはヨーの影響で減速


    motor_right->command(right_speed); // 正転
    motor_left->command(left_speed);   // 正転
}