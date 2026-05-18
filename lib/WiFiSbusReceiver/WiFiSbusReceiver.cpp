#include "WiFiSbusReceiver.hpp"

WiFiSbusReceiver::WiFiSbusReceiver(const char* ssid, const char* password, const char* hostname)
    : _ssid(ssid), _password(password), _hostname(hostname), _isFailsafe(true), _lastReceiveTime(0) {
    applyFailsafeValues();
}

void WiFiSbusReceiver::begin() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(_ssid, _password);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
    }

    // mDNSの開始 (sbus-wifi.local という名前でアクセス可能にする)
    if (MDNS.begin(_hostname)) {
        // 必要に応じてサービスを登録（任意）
        MDNS.addService("udp", "udp", _port);
    }

    _udp.begin(_port);
}

void WiFiSbusReceiver::update() {
    // WiFiが切れたら自動再接続（ESP32のWiFiスタックが自動で行うためチェックのみ）
    if (WiFi.status() != WL_CONNECTED) return;

    int packetSize = _udp.parsePacket();
    if (packetSize >= 6) {
        _udp.read(_packetBuffer, 6);
        decodePacket(_packetBuffer);
        _lastReceiveTime = millis();
        _isFailsafe = false;
    }

    if (!_isFailsafe && (millis() - _lastReceiveTime > _failsafeTimeout)) {
        applyFailsafeValues();
        _isFailsafe = true;
    }
}

void WiFiSbusReceiver::decodePacket(uint8_t* buf) {
    _channels[0] = (buf[0] | (buf[1] << 8)) & 0x07FF;
    _channels[1] = ((buf[1] >> 3) | (buf[2] << 5)) & 0x07FF;
    _channels[2] = ((buf[2] >> 6) | (buf[3] << 2) | (buf[4] << 10)) & 0x07FF;
    _channels[3] = ((buf[4] >> 1) | (buf[5] << 7)) & 0x07FF;
}

void WiFiSbusReceiver::applyFailsafeValues() {
    _channels[0] = 0;    _channels[1] = 1024;
    _channels[2] = 1024; _channels[3] = 1024;
}

uint16_t WiFiSbusReceiver::getChannel(uint8_t index) const {
    if (index < 4) return _channels[index];
    return 0;
}

bool WiFiSbusReceiver::isFailsafe() const { return _isFailsafe; }