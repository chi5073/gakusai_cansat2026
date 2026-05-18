#include "SBUS.hpp"

SBUSReceiver::SBUSReceiver(HardwareSerial& serial) 
  : _serial(serial), _failsafe(true), _newData(false) {
    // 配列の初期化
    for (int i = 0; i < 16; i++) {
        _channelData[i] = 0;
    }
}

void SBUSReceiver::begin(int rxPin, int txPin) {
    // S.BUSは100000bps、8N1、逆転シリアル (ESP32などのtrue設定)
    _serial.begin(100000, SERIAL_8N1, rxPin, txPin, true);
}

void SBUSReceiver::update() { // 引数が消えました
    uint8_t data_raw[25];
    getData(data_raw);
    
    if (_newData) {
        organizeData(data_raw);
        readSticks(); // 内部変数を更新
    }
}

void SBUSReceiver::readSticks() {
    // 内部の _stickData 配列に直接書き込む
    _stickData[0] = ((_channelData[2] - 368) * 100) / 1312;      // スロットル
    _stickData[1] = ((_channelData[0] - 1024) * 200) / 1312;     // 横転舵(エルロン)
    _stickData[2] = ((_channelData[1] - 1024) * 200) / -1312;    // 昇降舵(エレベーター)
    _stickData[3] = ((_channelData[3] - 1024) * 200) / 1312;     // 方向舵(ラダー)
}

// 外部から値を読み取るためのGetter関数
int SBUSReceiver::getThrottle() const { return _stickData[0]; }
int SBUSReceiver::getAileron() const  { return _stickData[1]; }
int SBUSReceiver::getElevator() const { return _stickData[2]; }
int SBUSReceiver::getRudder() const   { return _stickData[3]; }

uint16_t SBUSReceiver::getChannel(uint8_t ch) const {
    if (ch < 16) return _channelData[ch];
    return 0;
}

bool SBUSReceiver::isFailsafe() const {
    return _failsafe;
}

bool SBUSReceiver::hasNewData() const {
    return _newData;
}

void SBUSReceiver::getData(uint8_t* data_received) {
    _newData = false;
    uint8_t receiveddata[25];
    int count = 0;
  
    // 25バイト以上のとき、古いデータのため再読込
    while (24 < _serial.available() && count < 12) {
        count++;
        // S.BUSのフレームは0x0fで始まるので、0x0fが来るまで読み飛ばす
        while (24 < _serial.available() && _serial.read() != 0x0f);
  
        if (23 < _serial.available()) {
            receiveddata[0] = 0x0f; // 先頭は確定
            
            for (int i = 1; i < 25; i++) {
                receiveddata[i] = _serial.read();
            }
  
            // 0x00で終わっていなければデータ破損とみなす
            if (receiveddata[24] == 0x00) {
                _newData = true;
                for (int i = 0; i < 25; i++) {
                    data_received[i] = receiveddata[i];
                }
            }
        }
    }
}

void SBUSReceiver::organizeData(const uint8_t* data_rcvd) {
    for (int ch_num = 0; ch_num < 16; ch_num++) {
        int S_bit = 11 * ch_num + 8;
        int S_byte = S_bit / 8;
        uint32_t ch_data = 0;
        
        ch_data = ((uint32_t)data_rcvd[S_byte]) |
                  ((uint32_t)data_rcvd[S_byte+1] << 8) |
                  ((uint32_t)data_rcvd[S_byte+2] << 16);

        ch_data = (ch_data >> (S_bit % 8)) & 0x7FF; // 11ビット抽出

        _channelData[ch_num] = ch_data;
    }

    // Failsafe判定
    _failsafe = (data_rcvd[23] & 0x8) || (data_rcvd[0] < 15);
}