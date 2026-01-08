#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

struct wifiData
{
  //Without connection
  char ssid[33];
  int8_t rssi;
  String bssid;
  uint8_t encryptationType;
  int32_t channel;
  //WiFi connected
  String hostname;
  String dnsIP;
  String subNetMask;
  String localIP;
}

extern QueueHandle_t wifiQueue;

#endif // !CONFIG_H
