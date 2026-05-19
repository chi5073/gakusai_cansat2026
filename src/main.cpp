#include <Arduino.h>
#include "config.hpp"
#include "motor_driver.hpp"
#include "WiFiSbusReceiver.hpp"
#include "rover_controll.hpp"
#include "SBUS.hpp"

// グローバル空間での静的確保に変更
MotorDriver motor_right(Config::m_right_pin1, Config::m_right_pin2, Config::pwmChannelRight, Config::pwmFreq, Config::pwmResolution);
MotorDriver motor_left(Config::m_left_pin1, Config::m_left_pin2, Config::pwmChannelLeft, Config::pwmFreq, Config::pwmResolution);
WiFiSbusReceiver receiver(Config::ssid, Config::password, Config::host_name);
RoverController controller(Config::steering_gain, Config::throttle_gain, &motor_right, &motor_left);
SBUSReceiver sbusReceiver(Serial2);

void setup() {
  Serial.begin(115200);
  //receiver.begin();
  sbusReceiver.begin(Config::RX_PIN, Config::TX_PIN);
  
  motor_right.command(0); 
  motor_left.command(0); 
}

void loop() {
  //receiver.update();
  sbusReceiver.update();

  // 両方の通信手段のフェイルセーフを考慮する
  bool is_sbus_safe = !sbusReceiver.isFailsafe();
  //bool is_wifi_safe = !receiver.isFailsafe(); // WiFi側にもFailsafe実装を推奨
  if (is_sbus_safe) { 
    // throttleを -100 〜 100 の範囲にマッピング
    //int16_t throttle = map(receiver.getChannel(0), 0, 2047, -100, 100);
    //int16_t yaw      = map(receiver.getChannel(1), 0, 2047, -100, 100);
    int16_t throttle = sbusReceiver.getElevator(); // -100〜100のスロットル値
    int16_t yaw      = sbusReceiver.getRudder();  // -100〜100の旋回値
    //  Serial.printf("Throttle: %d, Yaw: %d\n", throttle, yaw); // デバッグ用に値を表示
    controller.command(yaw, throttle);
    delay(20); 
  } else {
    // フェイルセーフ時の確実な停止
    motor_right.stop();
    motor_left.stop();
    delay(100); 
  }
}