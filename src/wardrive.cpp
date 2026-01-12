// Local libs
#include "wardrive.h"
#include "config.h"
#include "indicator.h"

// Libs
#include <Arduino.h>
#include <WiFi.h>

QueueHandle_t WDQueue;

bool openNetworkfound = false;

void setupWardrive()
{
    WDQueue = xQueueCreate(10, sizeof(WardriveData));
}

bool startWardrive()
{
    int networks = WiFi.scanNetworks();

    for(int n = 0; n < networks; n++) {
        WardriveData data;

        wifi_auth_mode_t encryptationType = WiFi.encryptionType(n);

        strncpy(data.ssid, WiFi.SSID(n).c_str(), sizeof(data.ssid));
        data.rssi = WiFi.RSSI(n);

        // Send to the queue
        xQueueSend(WDQueue, &data, pdMS_TO_TICKS(10));

        if (encryptationType == WIFI_AUTH_OPEN)
        {
            String NetworkName = WiFi.SSID(n);
            DEBUG_PRINTLN("Open network found: ");
            DEBUG_PRINT(NetworkName);
            openNetworkfound = true;
        }
    }
    WiFi.scanDelete();
    return openNetworkfound;
}
