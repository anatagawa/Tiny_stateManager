#include "../libs/gamestate.h"

static void Pause_Init(void) {}
static void Pause_Enter(void) { TraceLog(LOG_INFO, "Pause entered"); }
static void Pause_Leave(void) { TraceLog(LOG_INFO, "Pause left"); }
static void Pause_Resume(void) { TraceLog(LOG_INFO, "Pause resumed"); }

static void Pause_Update(float dt) {
    if (IsKeyPressed(KEY_ENTER)) {
        GSM_Pop(&globalGSM); // return to previous state (GameState)
    }
}

static void Pause_Draw(void) {
    ClearBackground(BLANK);
    DrawText("PAUSE STATE", 100, 100, 20, YELLOW);
    DrawText("Press ENTER to resume", 100, 150, 20, WHITE);
}

STATE_DEFINE(PauseState, Pause_Init, Pause_Enter, Pause_Leave, Pause_Resume, Pause_Update, Pause_Draw);
