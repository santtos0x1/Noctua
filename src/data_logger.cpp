#include "data_logger.h"
#include "config.h"
#include <Arduino.h>
#include <SD.h>

wifiData receivedData;

bool initSD()
{
    // init your SD here
}

void logWiFiData()
{
    if (xQueueReceive(wifiQueue, &receivedData, portMAX_DELAY))
    {
        // log information in SD here
    }
}
