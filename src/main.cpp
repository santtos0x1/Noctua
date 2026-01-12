// Local libs
#include "config.h"
#include "fsm.h"
#include "wifi_scan.h"
#include "bt_scan.h"
#include "data_logger.h"

// Libs
#include <Arduino.h>

void setup()
{
    Serial.begin(115200);
    DEBUG_PRINTLN("Welcome to Noctua!");
    setupFSM();
}

void loop()
{
    runFSM();
}