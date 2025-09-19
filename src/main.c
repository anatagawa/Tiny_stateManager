#include "libs/common.h"
#include "libs/gamestate.h"

// Global GameState Manager
GamestateManager globalGSM;

// State declarations
STATE_DECLARE(MenuState);
STATE_DECLARE(GameState);
STATE_DECLARE(PauseState);

// Main entry point
int main(void) {
	SetConfigFlags(FLAG_MSAA_4X_HINT); // Anti-Aliasing 4x
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raylib Tiny Gamestate Example (Multi States)");
    SetTargetFPS(60);

	// Initialize GameState Manager
    GSM_Init(&globalGSM);

    // Start from MenuState
    GSM_SwitchTo(&globalGSM, &MenuState);

    while (!WindowShouldClose()) {
		// Update
        GSM_Update(&globalGSM, GetFrameTime());

		// Draw
        BeginDrawing();
		{
        	GSM_Draw(&globalGSM);
		}
        EndDrawing();
    }

	// De-initialization
    CloseWindow();
    return 0;
}
