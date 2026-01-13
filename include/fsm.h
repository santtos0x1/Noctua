#ifndef FSM_H
#define FSM_H

enum State
{
    IDLE,
    SCAN,
    PROCESS,
    WEB_SERVER,
    WARDRIVE_MODE
};

extern State currentState;

void setupFSM();
void runFSM();

#endif // !FSM_H