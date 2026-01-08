#ifndef FSM_H
#define FSM_H

enum State
{
    IDLE,
    SCAN,
    PROCESS,
    DONE
};

extern State currentState;

void fsmUpdate();
void fsmInit();

#endif // !FSM_H