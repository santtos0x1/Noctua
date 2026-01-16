#include "config.h"
#include "boot_manager.h"

#include <Arduino.h>

void logoInit()
{
    DEBUG_PRINTLN("");
    DEBUG_PRINTF("%s      ▄▄      ▄▄▄                          ", CLR_GREEN);
    DEBUG_PRINTLN("      ██▄    ██▀             █▄            ");
    DEBUG_PRINTLN("      ███▄   ██             ▄██▄           ");
    DEBUG_PRINTLN("      ██ ▀█▄██ ▄███▄ ▄███▀  ██ ██ ██ ▄▀▀█▄ ");
    DEBUG_PRINTLN("      ██   ▀██ ██ ██ ██     ██ ██ ██ ▄█▀██ ");
    DEBUG_PRINTLN("    ▀██▀    ██▄▀███▀▄▀███▄▄ ██▄▀██▀█▄▀█▄██ %s", CLR_RESET);
    DEBUG_PRINTLN("");
    DEBUG_PRINTLN("      [ SYSTEM ONLINE - WIRELESS SNIFFER ]     ");
    DEBUG_PRINTLN("      - %sVERSION: 1.0%s - OPEN SOURCE PROJECT -   ", CLR_YELLOW, CLR_RESET);
    DEBUG_PRINTLN("-----------------------------------------------");
}

void configCheck()
{
    DEBUG_PRINTF("%s[BOOT]%s Checking hardware & modules...", CLR_GREEN, CLR_RESET);

    DEBUG_PRINT(" > WiFi Stack:      ");
    #if ENABLE_WIFI
        DEBUG_PRINTF("%s[ ENABLED  ]%s\n", CLR_GREEN, CLR_RESET);
    #else
        DEBUG_PRINTF("%s[ DISABLED  ]%s\n", CLR_RED, CLR_RESET);
    #endif

    DEBUG_PRINT(" > Bluetooth Stack: ");
    #if ENABLE_BT
        DEBUG_PRINTF("%s[ ENABLED  ]%s\n", CLR_GREEN, CLR_RESET);
    #else
        DEBUG_PRINTF("%s[ DISABLED  ]%s\n", CLR_RED, CLR_RESET);
    #endif

    DEBUG_PRINT(" > SD Storage:     ");
    #if ENABLE_SD
        DEBUG_PRINTF("%s[ ENABLED  ]%s\n", CLR_GREEN, CLR_RESET);
    #else
        DEBUG_PRINTF("%s[ DISABLED  ]%s\n", CLR_RED, CLR_RESET);
    #endif

    DEBUG_PRINTF(" > Internal RAM:    %d KB Free\n", ESP.getFreeHeap() / 1024);
    DEBUG_PRINTF(" > CPU Frequency: %d\n", ESP.getCpuFreqMHz());
    DEBUG_PRINTF(" > Microcontroller SDK Version: %s\n", ESP.getSdkVersion());
    DEBUG_PRINTF(" > Chip model: %s\n", ESP.getChipModel());

    // System Health
    DEBUG_PRINTLN("-----------------------------------------------");

    DEBUG_PRINTF(" > HEAP Status:     %d KB Free\n", ESP.getFreeHeap() / 1024);
    DEBUG_PRINTF(" > Min Free Heap:   %d KB (Ever)\n", ESP.getMinFreeHeap() / 1024);
    DEBUG_PRINTF(" > PSRAM Available: %s%s%S\n", 
        ESP.getPsramSize() > 0 ? CLR_GREEN : CLR_RED, 
        ESP.getPsramSize() > 0 ? "YES" : "NO",
        CLR_RESET
    );

    DEBUG_PRINTLN("-----------------------------------------------");
}