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
    DEBUG_PRINTLN(CLR_YELLOW "Starting SD diagnostic..." CLR_RESET);
    
    // Retrieves the physical interface type (e.g., SDSC, SDHC, MMC)
    DEBUG_PRINTLN(CLR_YELLOW "Probing card hardware..." CLR_RESET);
    sdcard_type_t cardType = SD.cardType();

    // Verification: If cardType is 0 (CARD_NONE), communication failed
    if(cardType == CARD_NONE)
    {
        DEBUG_PRINTLN(CLR_RED "Critical Error: SD Card not detected!" CLR_RESET);
        return false;
    }

    DEBUG_PRINTLN(CLR_GREEN "SD Status: Healthy and Mounted." CLR_RESET);
    return true;
}