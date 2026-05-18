#pragma once

#include <WiFi.h>
#include <WiFiUdp.h>
#include <ESPmDNS.h> // 追加

class WiFiSbusReceiver {
    public:
        // 第3引数をホスト名（例: "sbus-wifi"）に変更
        WiFiSbusReceiver(const char* ssid, const char* password, const char* hostname);

        void begin();
        void update();
        uint16_t getChannel(uint8_t index) const;
        bool isFailsafe() const;

    private:
        const char* _ssid;
        const char* _password;
        const char* _hostname; // 追加
        uint16_t _port = 12345;

        WiFiUDP _udp;
        uint16_t _channels[4];
        uint8_t _packetBuffer[6];
        unsigned long _lastReceiveTime;
        const unsigned long _failsafeTimeout = 500;
        bool _isFailsafe;

        void decodePacket(uint8_t* buf);
        void applyFailsafeValues();
};