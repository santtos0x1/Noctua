#ifndef DATA_LOGGER_H
#define DATA_LOGGER_H

#include "config.h"
#include <Arduino.h>
#include <SD.h>

void setupSD();
void logWiFiData();
void logBTData();
void logWDData();

#endif // !DATA_LOGGER_H
