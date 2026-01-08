#include "fsm.h"

void fsmInit()
{
    currentState = IDLE;
}

void fsmUpdate()
{
    switch(currentState)
    {
        case IDLE:
            currentState = SCAN;
            break;

        case SCAN:
            currentState = PROCESS;
            break;
        
        case PROCESS:
            currentState = DONE;
            break;
    
        case DONE:
            currentState = IDLE;
            break;
    }
}