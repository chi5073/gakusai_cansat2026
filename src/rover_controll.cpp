#include "rover_controll.hpp"

RoverController::RoverController(float steering_gain, float throttle_gain, MotorDriver* motor_right, MotorDriver* motor_left){
    this->_steering_gain = steering_gain;
    this->_throttle_gain = throttle_gain;
    this->motor_right = motor_right;
    this->motor_left = motor_left;
    this->motor_right->command(0); 
    this->motor_left->command(0); 
}

void RoverController::setup(){
}

void RoverController::command(int16_t yaw, int16_t throttle){
    // throttle: -100(フルバック) 〜 100(フルスロットル) に変更されている前提
    // yaw: -100(左旋回) 〜 100(右旋回)

    // 差動二輪（Differential Drive）の計算
    float right_target = (this->_throttle_gain * throttle) + (this->_steering_gain * yaw);
    float left_target  = (this->_throttle_gain * throttle) - (this->_steering_gain * yaw);

    // 計算結果を -255 〜 255 のPWM出力範囲にスケール

    // ゲインによって100を超える場合があるため、constrainで制限する
    int16_t right_pwm = constrain(map(right_target, -300, 300, -255, 255), -255, 255);
    int16_t left_pwm  = constrain(map(left_target, -300, 300, -255, 255), -255, 255);

    Serial.printf("targetRight, targetLeft: %.2f, %.2f, rightPwm: %d, leftPwm: %d, throttle: %d, yaw: %d\n", right_target, left_target, right_pwm, left_pwm, throttle, yaw); // デバッグ用に生のターゲット値を表示
    if(right_pwm >=0){
        motor_right->command(right_pwm);
    } else {
        motor_right->command(-right_pwm, false); // 逆転させるために符号を反転
    }   
    if(left_pwm >=0){
        motor_left->command(left_pwm);
    } else {
        motor_left->command(-left_pwm, false); // 逆転させるために符号を反転
    }
}