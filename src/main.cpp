#include <Arduino.h>
#include "config.h"
#include "fsm.h"
#include "wifi_scan.h"
#include "bt_scan.h"
#include "data_logger.h"

void setup()
{
    Serial.begin(115200);
    setupFSM();
}

void loop()
{
    runFSM();
}