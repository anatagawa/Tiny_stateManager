#include "../libs/gamestate.h"

// Pause Variables
State *previous = NULL;

static void Pause_Init(void) {
    previous = GSM_GetPrevious(&globalGSM); // get previous state (GameState)
}

static void Pause_Enter(void) { TraceLog(LOG_INFO, "Pause entered"); }
static void Pause_Leave(void) { TraceLog(LOG_INFO, "Pause left"); }
static void Pause_Resume(void) { TraceLog(LOG_INFO, "Pause resumed"); }

static void Pause_Update(float dt) {
    if (IsKeyPressed(KEY_ENTER)) {
        GSM_Pop(&globalGSM); // return to previous state (GameState)
    }
}

static void Pause_Draw(void) {
    // Draw previous state in background
    if (previous && previous->draw) previous->draw();

    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));

    DrawText("PAUSE STATE", (GetScreenWidth() / 2) - (MeasureText("PAUSE STATE", 20) / 2), 100, 20, YELLOW);
    DrawText("Press ENTER to resume", (GetScreenWidth() / 2) - (MeasureText("Press ENTER to resume", 20) / 2), 150, 20, WHITE);
}

STATE_DEFINE(PauseState, Pause_Init, Pause_Enter, Pause_Leave, Pause_Resume, Pause_Update, Pause_Draw);
