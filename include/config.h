#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// === DEFINES ===
// Delay templates
#define LOW_DELAY 100
#define LMID_DELAY 300
#define MID_DELAY 500
#define HMID_DELAY 1000
#define HIGH_DELAY 2000
#define IDLE_DELAY 3000

// bt_scan.cpp
#define scanTime 5

// data_logger.cpp
#define RAM_FLUSH_LIM 5
#define SD_CS_PIN 5

// fsm.cpp
#define BTN_A_PINOUT 14
#define BTN_B_PINOUT 16
#define BTN_C_PINOUT 18
#define BUILT_IN_LED 2

// wifi_scan.cpp
#define CONN_TIMEOUT_MS (10 * 1000)

// http_server.cpp
#define WEB_SERVER_PORT 80
#define HND_BUFFER_SIZE 512
#define SERVER_ATTEMPTS_LIMIT 20
// === === === ===

// === Structs/Queue ===
struct WiFiData
{
  //Without connection
  char ssid[33];
  int32_t rssi;
  char bssid[20];
  uint8_t encryptationType;
  int channel;
  // Connected
  char hostname[33];
  char dnsIP[16];
  char subNetMask[16];
  char localIP[16];
  char dhcp[20];
};

struct BTData
{
    char name[33];
    char address[18];
    int rssi;        
    char addressType[20];
    int channel;
};

struct WardriveData
{
  char ssid[33];
  int32_t rssi;
};

extern QueueHandle_t WiFiQueue;
extern QueueHandle_t BTQueue;
extern QueueHandle_t WDQueue;
// === === === ===

#endif // !CONFIG_H
