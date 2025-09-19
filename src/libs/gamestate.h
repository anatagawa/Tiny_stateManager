#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "common.h"

//-----------------------------------------------------------------------------------------------
// Functions
void GSM_Init(GamestateManager* gsm);
void GSM_SwitchTo(GamestateManager* gsm, State* to);
void GSM_Push(GamestateManager* gsm, State* to);
void GSM_Pop(GamestateManager* gsm);
void GSM_Update(GamestateManager* gsm, float dt);
void GSM_Draw(GamestateManager* gsm);

//-----------------------------------------------------------------------------------------------
// Macro helper to declare states easily
#define STATE_DECLARE(name) extern State name

#define STATE_DEFINE(name, initFunc, enterFunc, leaveFunc, resumeFunc, updateFunc, drawFunc) \
    State name = { initFunc, enterFunc, leaveFunc, resumeFunc, updateFunc, drawFunc }


// --- Shortcuts for state switching/pushing/popping ---
// Auto-log + auto-extern + switch
#define GS_SWITCH(name) do { extern State name; GSM_SwitchTo(&globalGSM, &name); } while(0)

// Push a state on top
#define GS_PUSH(name) do { extern State name; GSM_Push(&globalGSM, &name); } while(0)

// Pop current state
#define GS_POP() do { GSM_Pop(&globalGSM); } while(0)

#endif
// GAMESTATE_H
