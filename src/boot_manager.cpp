#include "config.h"
#include "boot_manager.h"

#include <Arduino.h>

void logoInit()
{
    DEBUG_PRINTLN(F(""));
    DEBUG_PRINTF(F("%s      ▄▄      ▄▄▄                          "), CLR_GREEN);
    DEBUG_PRINTLN(F("      ██▄    ██▀             █▄            "));
    DEBUG_PRINTLN(F("      ███▄   ██             ▄██▄           "));
    DEBUG_PRINTLN(F("      ██ ▀█▄██ ▄███▄ ▄███▀  ██ ██ ██ ▄▀▀█▄ "));
    DEBUG_PRINTLN(F("      ██   ▀██ ██ ██ ██     ██ ██ ██ ▄█▀██ "));
    DEBUG_PRINTLN(F("    ▀██▀    ██▄▀███▀▄▀███▄▄ ██▄▀██▀█▄▀█▄██ %s"), CLR_RESET);
    DEBUG_PRINTLN(F(""));
    DEBUG_PRINTLN(F("      [ SYSTEM ONLINE - WIRELESS SNIFFER ]     "));
    DEBUG_PRINTLN(F("      - %sVERSION: 1.0%s - OPEN SOURCE PROJECT -   "), CLR_YELLOW, CLR_RESET);
    DEBUG_PRINTLN(F("-----------------------------------------------"));
}

void configCheck()
{
    DEBUG_PRINTF(F("%s[BOOT]%s Checking hardware & modules..."), CLR_GREEN, CLR_RESET);

    DEBUG_PRINT(F("> WiFi Stack:      "));
    #if ENABLE_WIFI
        DEBUG_PRINTF(F("(%s[ ENABLED  ]%s\n"), CLR_GREEN, CLR_RESET);
    #else
        DEBUG_PRINTF(F("%s[ DISABLED  ]%s\n"), CLR_RED, CLR_RESET);
    #endif

    DEBUG_PRINT(F("> Bluetooth Stack: "));
    #if ENABLE_BT
        DEBUG_PRINTF(F("%s[ ENABLED  ]%s\n"), CLR_GREEN, CLR_RESET);
    #else
        DEBUG_PRINTF(F("%s[ DISABLED  ]%s\n"), CLR_RED, CLR_RESET);
    #endif

    DEBUG_PRINT(F("> SD Storage:     "));
    #if ENABLE_SD
        DEBUG_PRINTF(F("%s[ ENABLED  ]%s\n", CLR_GREEN, CLR_RESET));
    #else
        DEBUG_PRINTF(F("%s[ DISABLED  ]%s\n", CLR_RED, CLR_RESET));
    #endif

    DEBUG_PRINTF(F(" > Internal RAM:    %d KB Free\n", ESP.getFreeHeap() / 1024));
    DEBUG_PRINTF(F(" > CPU Frequency: %d\n", ESP.getCpuFreqMHz()));
    DEBUG_PRINTF(F(" > Microcontroller SDK Version: %s\n", ESP.getSdkVersion()));
    DEBUG_PRINTF(F(" > Chip model: %s\n", ESP.getChipModel()));

    // System Health
    DEBUG_PRINTLN(F("-----------------------------------------------"));

    DEBUG_PRINTF(F(" > HEAP Status:     %d KB Free\n", ESP.getFreeHeap() / 1024));
    DEBUG_PRINTF(F(" > Min Free Heap:   %d KB (Ever)\n", ESP.getMinFreeHeap() / 1024));
    DEBUG_PRINTF(F(" > PSRAM Available: %s%s%S\n"), 
        ESP.getPsramSize() > 0 ? CLR_GREEN : CLR_RED, 
        ESP.getPsramSize() > 0 ? "YES" : "NO",
        CLR_RESET
    );

    DEBUG_PRINTLN(F("-----------------------------------------------"));
}