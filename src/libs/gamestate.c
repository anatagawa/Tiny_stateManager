#include "gamestate.h"

void GSM_Init(GamestateManager* gsm) {
    gsm->top = -1;
}

void GSM_SwitchTo(GamestateManager* gsm, State* to) {
    if (gsm->top >= 0 && gsm->stack[gsm->top]->leave)
        gsm->stack[gsm->top]->leave();

    gsm->top = 0;
    gsm->stack[gsm->top] = to;

    if (to->init) to->init();
    if (to->enter) to->enter();
}

void GSM_Push(GamestateManager* gsm, State* to) {
    assert(gsm->top < MAX_STATES - 1 && "Stack overflow!");
    gsm->stack[++gsm->top] = to;

    if (to->init) to->init();
    if (to->enter) to->enter();
}

void GSM_Pop(GamestateManager* gsm) {
    assert(gsm->top > 0 && "No more states to pop!");

    State* pre = gsm->stack[gsm->top--];
    if (pre->leave) pre->leave();

    State* resumed = gsm->stack[gsm->top];
    if (resumed->resume) resumed->resume();
}

void GSM_Update(GamestateManager* gsm, float dt) {
    if (gsm->top >= 0 && gsm->stack[gsm->top]->update)
        gsm->stack[gsm->top]->update(dt);
}

void GSM_Draw(GamestateManager* gsm) {
    if (gsm->top >= 0 && gsm->stack[gsm->top]->draw)
        gsm->stack[gsm->top]->draw();
}
