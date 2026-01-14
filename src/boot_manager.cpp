#include "config.h"
#include "boot_manager.h"

#include <Arduino.h>

void logoInit()
{
    DEBUG_PRINTLN("");
    DEBUG_PRINTLN("      ▄▄      ▄▄▄                          ");
    DEBUG_PRINTLN("      ██▄    ██▀             █▄            ");
    DEBUG_PRINTLN("      ███▄   ██             ▄██▄           ");
    DEBUG_PRINTLN("      ██ ▀█▄██ ▄███▄ ▄███▀  ██ ██ ██ ▄▀▀█▄ ");
    DEBUG_PRINTLN("      ██   ▀██ ██ ██ ██     ██ ██ ██ ▄█▀██ ");
    DEBUG_PRINTLN("    ▀██▀    ██▄▀███▀▄▀███▄▄ ██▄▀██▀█▄▀█▄██ ");
    DEBUG_PRINTLN("");
    DEBUG_PRINTLN("      [ SYSTEM ONLINE - WIRELESS SNIFFER ]     ");
    DEBUG_PRINTLN("      - VERSION: 1.0 - OPEN SOURCE PROJECT -   ");
    DEBUG_PRINTLN("-----------------------------------------------");
}

void configCheck()
{
    DEBUG_PRINTLN("[BOOT] Checking hardware & modules...");

    DEBUG_PRINT(" > WiFi Stack:      ");
    #if ENABLE_WIFI
        DEBUG_PRINTLN("[ ENABLED  ]");
    #else
        DEBUG_PRINTLN("[ DISABLED ]");
    #endif

    DEBUG_PRINT(" > Bluetooth Stack: ");
    #if ENABLE_BT
        DEBUG_PRINTLN("[ ENABLED  ]");
    #else
        DEBUG_PRINTLN("[ DISABLED ]");
    #endif

    DEBUG_PRINT(" > SD Storage:     ");
    #if ENABLE_SD
        DEBUG_PRINTLN("[ ENABLED  ]");
    #else
        DEBUG_PRINTLN("[ DISABLED ]");
    #endif

    DEBUG_PRINTF(" > Internal RAM:    %d KB Free\n", ESP.getFreeHeap() / 1024);
    
    DEBUG_PRINTLN("-----------------------------------------------");
}