#ifndef FSM_H
#define FSM_H

enum State
{
    IDLE,
    SCAN,
    WARDRIVE_MODE,
    WEB_SERVER
};

extern State currentState;

void setupFSM();
void runFSM();

#endif // !FSM_H 