// Local libs
#include "fsm.h"
#include "bt_scan.h"
#include "wifi_scan.h"
#include "data_logger.h"
#include "watchdog.h"
#include "indicator.h"
#include "http_server.h"
#include "wardrive.h"
#include "config.h"

// Libs
#include <Arduino.h>

bool btnALastState = HIGH;
bool btnBLastState = HIGH;
bool btnCLastState = HIGH;

String scanMode;

bool serverStatus = false;

void setupFSM()
{
    // Initing modules
    #if ENABLE_WIFI
        setupWiFi();
    #endif
    #if ENABLE_BT
        setupBT();
    #endif    
    #if ENABLE_SD
        setupSD();
    #endif    
    setupIndicator(BUILT_IN_LED);
    serverStatus = startServer();
    setupWardrive();

    // Setting PINs
    pinMode(BTN_A_PINOUT, INPUT_PULLUP);
    pinMode(BTN_B_PINOUT, INPUT_PULLUP);
    pinMode(BTN_C_PINOUT, INPUT_PULLUP);

    // Start state
    currentState = IDLE;
}

void runFSM()
{
    bool btnAPressed = (digitalRead(BTN_A_PINOUT) == LOW && btnALastState == HIGH);
    bool btnBPressed = (digitalRead(BTN_B_PINOUT) == LOW && btnBLastState == HIGH);
    bool btnCPressed = (digitalRead(BTN_C_PINOUT) == LOW && btnCLastState == HIGH);

    switch(currentState)
    {
        case IDLE:
        {
            if(digitalRead(BTN_A_PINOUT) == LOW)
            {
                delay(LOW_DELAY);
                unsigned long gap = millis() + 400;
                bool doubleClicked = false;

                while(digitalRead(BTN_A_PINOUT) == LOW);

                while(millis() < gap) {
                    if (digitalRead(BTN_A_PINOUT) == LOW) {
                        doubleClicked = true;
                        break;
                    }
                }

                if(doubleClicked)
                {
                    scanMode = "WD";
                    currentState = WARDRIVE_MODE;
                    showOn(BUILT_IN_LED);
                } else {
                    scanMode = "WF";
                    currentState = SCAN;
                }
                break;
            } else if(btnBPressed) {
                scanMode = "BT";
                currentState = SCAN;
                break;
            } else if(btnCPressed) {
                scanMode = "WS";
                currentState = WEB_SERVER;
                break;
            } 
        }

        case SCAN:
        {
            DEBUG_PRINTLN("Current FSM state: SCAN");
            #if ENABLE_SD
                bool SDReport = SDDoctor();            
                if(!SDReport)
                {
                    showError(BUILT_IN_LED);
                    //Try to restart the SD
                    setupSD();
                    currentState = IDLE;
                    break;
                }
            #endif
            
            if(scanMode == "WF")
            {
                #if ENABLE_WIFI
                    WiFiSniffer();
                    currentState = PROCESS;
                    showSuccess(BUILT_IN_LED);
                    break;
                #else
                    currentState = IDLE;
                    break;
                #endif

            } else if(scanMode == "BT") {
                #if ENABLE_BT
                    BTSniffer();
                    currentState = PROCESS;
                    showSuccess(BUILT_IN_LED);
                    break;
                #else
                    currentState = IDLE;
                    break;
                #endif
            }
        }
        
        case PROCESS:
        {
            DEBUG_PRINTLN("Current FSM state: PROCESS");

            if(scanMode == "WF")
            {
                #if ENABLE_SD
                    logWiFiData();
                    currentState = IDLE;
                    break;
                #else
                    currentState = IDLE;
                    break;
                #endif
            } else if (scanMode == "BT") {
                #if ENABLE_SD
                    logBTData();
                    currentState = IDLE;
                    break;
                #else
                    currentState = IDLE;
                    break;
                #endif
            }

            showProcessing(BUILT_IN_LED);
        }

        case WEB_SERVER:
        {
            DEBUG_PRINTLN("Current FSM state: WEB_SERVER");
            
            if(serverStatus && scanMode == "WS")
            {
                showSuccess(BUILT_IN_LED);
                serverCFG();
            } else {
                showError(BUILT_IN_LED);
            }

            currentState = IDLE;
            break;
        }

        case WARDRIVE_MODE:
        {
            startWardrive();
            logWDData();
        }
    }

    btnALastState = digitalRead(BTN_A_PINOUT);
    btnBLastState = digitalRead(BTN_B_PINOUT);
    btnCLastState = digitalRead(BTN_C_PINOUT);
}