#ifndef COMMON_H
#define COMMON_H

// Raylib
#include <raylib.h>

// Standard
#include <assert.h>
#include <stddef.h>

// Config globale
#define MAX_STATES 8
#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480

//-----------------------------------------------------------------------------------------------
// GameState definition
typedef void (*StateFuncVoid)(void);
typedef void (*StateFuncUpdate)(float);

typedef struct {
    StateFuncVoid   init;
    StateFuncVoid   enter;
    StateFuncVoid   leave;
    StateFuncVoid   resume;
    StateFuncUpdate update;
    StateFuncVoid   draw;
} State;

//-----------------------------------------------------------------------------------------------
// Manager
typedef struct {
    State* stack[MAX_STATES];
    int top;
} GamestateManager;

extern GamestateManager globalGSM;

#endif
// COMMON_H
