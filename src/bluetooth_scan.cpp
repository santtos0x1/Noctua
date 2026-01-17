// Local libs
#include "config.h"
#include "bluetooth_scan.h"
#include "utils.h"

// Libs
#include <BLEAdvertisedDevice.h>
#include <BLEDevice.h>
#include <Arduino.h>
#include <BLEScan.h>

QueueHandle_t BTQueue;
BLEScan *pBLEscan;

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
    void onResult(BLEAdvertisedDevice advertisedDevice)
    {
        BTData data;

        /*
            Clears the data struct before populating it to ensure memory integrity.
        */
        DEBUG_PRINTLN(F(CLR_YELLOW "Cleaning data from the struct..." CLR_RESET));
        memset(&data, 0, sizeof(BTData));

        /*
            Retrieves device metadata.
        */
        bool haveName =  advertisedDevice.haveName();
        DEBUG_PRINTLN(F("Getting device name..."));
        if (haveName)
        {
            // CORREÇÃO: F() aplicado apenas na string de formato e cor
            DEBUG_PRINTF(F(CLR_GREEN "Device has a name: %s\n" CLR_RESET), advertisedDevice.getName().c_str());
            strncpy(data.name, advertisedDevice.getName().c_str(), sizeof(data.name) - 1);
        } else {
            DEBUG_PRINTLN(F(CLR_RED "Device does not have a name!" CLR_RESET));
            strncpy(data.name, "Unknown", sizeof(data.name) - 1);
        }

        DEBUG_PRINTLN(F("Getting device address..."));
        strncpy(data.address, advertisedDevice.getAddress().toString().c_str(), sizeof(data.address) - 1);

        DEBUG_PRINTLN(F("Getting device RSSI..."));
        data.rssi = advertisedDevice.getRSSI();

        DEBUG_PRINTLN(F("Getting device address type..."));
        esp_ble_addr_type_t type = advertisedDevice.getAddressType();
        switch (type)
        {
            case BLE_ADDR_TYPE_PUBLIC:
            {
                strncpy(data.addressType, "Public",     sizeof(data.addressType) - 1);
                break;
            }
            case BLE_ADDR_TYPE_RANDOM:
            {
                strncpy(data.addressType, "Random",     sizeof(data.addressType) - 1);
                break;
            }
            case BLE_ADDR_TYPE_RPA_PUBLIC:
            {
                strncpy(data.addressType, "RPA_Public", sizeof(data.addressType) - 1);
                break;
            }
            case BLE_ADDR_TYPE_RPA_RANDOM:
            {
                strncpy(data.addressType, "RPA_Random", sizeof(data.addressType) - 1);
                break;
            }
            default:
            {
                strncpy(data.addressType, "Unknown",    sizeof(data.addressType) - 1);
                break;
            }
        }

        DEBUG_PRINTLN(F("Defining channel..."));
        data.channel = 0; 

        /* Sends the populated struct to the queue receiver. */
        DEBUG_PRINTLN(F("Sending to the queue..."));
        if (xQueueSend(BTQueue, &data, pdMS_TO_TICKS(100)) == pdPASS)
        {
            DEBUG_PRINTLN(F(CLR_GREEN "Data sent to BTQueue successfully." CLR_RESET));
        } else {
            DEBUG_PRINTLN(F(CLR_RED "Bluetooth Queue Full! Data lost." CLR_RESET));
        }
    }
};

void setupBluetooth()
{
    DEBUG_PRINTLN(F(CLR_YELLOW "Creating the queue..." CLR_RESET));
    #if ASYNC_SD_HANDLER
        BTQueue = xQueueCreate(DUALCORE_MAX_XQUEUE, sizeof(BTData));
    #else
        BTQueue = xQueueCreate(SINGLECORE_MAX_XQUEUE, sizeof(BTData));
    #endif

    DEBUG_PRINTLN(F(CLR_YELLOW "Starting bluetooth modules..." CLR_RESET));
    BLEDevice::init("");

    pBLEscan = BLEDevice::getScan(); 
    pBLEscan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEscan->setActiveScan(true);
    pBLEscan->setInterval(100); 
    pBLEscan->setWindow(99);    
}

void BluetoothSniffer()
{
    DEBUG_PRINTLN(F(CLR_YELLOW "Starting bluetooth scan..." CLR_RESET));
    pBLEscan->start(SCAN_TIME, false);
    pBLEscan->clearResults(); 
    DEBUG_PRINTLN(F(CLR_GREEN "BLE Scan done!" CLR_RESET));
}