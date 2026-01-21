#ifndef UTILS_H
#define UTILS_H

#include "config.h"


#if SYS_CFG_USE_ANSI_COLORS
    #define CLR_RESET  "\e[0m"
    #define CLR_RED    "\e[31m"
    #define CLR_GREEN  "\e[32m"
    #define CLR_YELLOW "\e[33m"
    #define CLR_CYN    "\e[36m"
#else
    #define CLR_RESET  ""
    #define CLR_RED    ""
    #define CLR_GREEN  ""
    #define CLR_YELLOW ""
    #define CLR_CYN    ""
#endif

// wifi_scan.cpp
#define CONN_TIMEOUT_MS       (10 * 1000)
#define GET_RSSI_QUALITY(rssi) ( \
    (rssi >= -50) ? "Strong" :  \
    (rssi >= -70) ? "Medium" :  \
    (rssi >= -85) ? "Weak"   : "V.Weak" \
)

//fsm.cpp
#define GET_STATE(index) ( \
    (index == 0) ? "WiFi"       : \
    (index == 1) ? "Bluetooth"  : \
    (index == 2) ? "Wardrive"   : \
    (index == 3) ? "Web Server" : "" \
)

//bluetooth_scan.cpp
#define GET_ADDR_TYPE(type) \
    ((type == BLE_ADDR_TYPE_PUBLIC)     ? "Public"     : \
     (type == BLE_ADDR_TYPE_RANDOM)     ? "Random"     : \
     (type == BLE_ADDR_TYPE_RPA_PUBLIC) ? "RPA_Public" : \
     (type == BLE_ADDR_TYPE_RPA_RANDOM) ? "RPA_Random" : "Unknown")
#endif // !UTILS_h
