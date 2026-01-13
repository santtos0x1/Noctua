// Local libs
#include "data_logger.h"
#include "wifi_scan.h"
#include "bt_scan.h"
#include "config.h"
#include "fsm.h"

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