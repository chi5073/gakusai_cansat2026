#pragma once
#include <Arduino.h>

namespace Config {
    // モーターのピン設定
    const int m_right_pin1 = 17;
    const int m_right_pin2 = 16;
    const int m_left_pin1 = 22;
    const int m_left_pin2 = 21;

    // PWM設定
    const uint32_t pwmFreq = 20000; // PWM周波数
    const uint8_t pwmResolution = 8; // PWM解像度
    const uint8_t pwmChannelRight = 0; // 右モーターのPWMチャンネル
    const uint8_t pwmChannelLeft = 1; // 左モーターのPWM

    // コントローラーのゲイン設定
    const float steering_gain = 0.5; // 旋回の影響を
    const float throttle_gain = 2.5; // スロットルの影響を

    //Wifi設定
    const char* ssid = "X"; // ホットスポットのSSIDに書き換えてください
    const char* password = "chio1222"; // パスワードに書き換えてください
    const int localPort = 4210;      // 通信ポート番号
    const char* host_name = "sbus-wifi"; // ホスト名

    //S.BUSの受信に使うSerial1の設定。
    const int RX_PIN = 5; // S.BUSの受信
    const int TX_PIN = 23; // S.BUSの送信(浮かせとく)

}