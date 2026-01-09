#ifndef FSM_H
#define FSM_H

#include <Arduino.h>

enum State
{
    IDLE,
    SCAN,
    PROCESS
};

extern State currentState;

void setupFSM();
void runFSM();

#endif // !FSM_H