#include <Arduino.h>
#include "config.hpp"
#include "motor_driver.hpp"
#include "WiFiSbusReceiver.hpp"
#include "rober_controll.hpp"

MotorDriver* motor_right = nullptr;
MotorDriver* motor_left = nullptr;
WiFiSbusReceiver* receiver = nullptr;
RoberController* controller = nullptr;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  receiver = new WiFiSbusReceiver(Config::ssid, Config::password, Config::host_name);
  receiver->begin();
  motor_right = new MotorDriver(Config::m_right_pin1, Config::m_right_pin2, Config::pwmChannelRight, Config::pwmFreq, Config::pwmResolution);
  motor_left = new MotorDriver(Config::m_left_pin1, Config::m_left_pin2, Config::pwmChannelLeft, Config::pwmFreq, Config::pwmResolution);
  controller = new RoberController(Config::steering_gain, Config::throttle_gain, motor_right, motor_left);
  motor_right->command(0); // 右モーターを正転で速度254/255=0.996で回す
  motor_left->command(0); // 左モーターを正転で速度200/255=0.784で回す
}

void loop() {
  // put your main code here, to run repeatedly:
  // データ受信とフェイルセーフ監視を実行
  receiver->update();

  // 取得した11bit(0-2047)を8bit(0-255)やPWM幅に変換
  // ※チャンネルの順番はPython側の送信順と合わせてください
  uint16_t throttle = map(receiver->getChannel(0), 0, 2047, 0, 100);
  int16_t yaw      = map(receiver->getChannel(3), 0, 2047, -100, 100);

  // コマンドをローバーコントローラーに送信
  controller->command(yaw, throttle);
  delay(20); // 50Hz程度のループを維持
}
