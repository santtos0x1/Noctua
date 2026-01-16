// Local Libs
#include "watchdog.h"
#include "config.h"

// Libs
#include <Arduino.h>
#include <SD.h>

/**
 * Performs a hardware integrity check on the SD Card.
 * Validates if the media is physically present and responding.
 * Returns true if the card is ready for I/O operations.
 */
bool SdHealthyChecker()
{
    DEBUG_PRINTLN("Starting SD diagnostic...");
    
    // Retrieves the physical interface type (e.g., SDSC, SDHC, MMC)
    DEBUG_PRINTLN("Probing card hardware...");
    sdcard_type_t cardType = SD.cardType();

    // Verification: If cardType is 0 (CARD_NONE), communication failed
    if(cardType == CARD_NONE)
    {
        DEBUG_PRINTLN("Critical Error: SD Card not detected!");
        return false;
    }

    DEBUG_PRINTLN("SD Status: Healthy and Mounted.");
    return true;
}