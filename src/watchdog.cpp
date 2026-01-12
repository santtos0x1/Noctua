// Local Libs
#include "watchdog.h"
#include "config.h"

// Libs
#include <Arduino.h>
#include <SD.h>

bool SDDoctor()
{
    sdcard_type_t cardType = SD.cardType();
    if(cardType == CARD_NONE)
    {
        DEBUG_PRINTLN("Error: Card not found!");
        return false;
    }
    return true;
}