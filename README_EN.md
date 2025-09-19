# 🎮 Tiny State Manager in C with [Raylib](https://www.raylib.com/)

A small **Game State Manager** written in **pure C** for Raylib, designed for **maximum compatibility** (PC, Dreamcast, retro consoles).  
It is minimal, readable, and extensible: perfect for your games, demos, or prototypes.


## 🚀 Features

- ⚡ **Pure C** (no C++, no classes → portable everywhere)
- 🧩 **Modular states** (menu, game, pause, etc.)
- 📦 **State transitions**:
  - `GSM_SwitchTo()` → switch to a new state  
  - `GSM_Push()` → push a new state on top  
  - `GSM_Pop()` → go back to the previous state
- 🔄 **Standardized callbacks** for each state:
  - `init` → initialization (resource loading)
  - `enter` → when entering the state
  - `update` → game logic
  - `draw` → rendering
  - `leave` → leaving the state (resource cleanup)
  - `resume` → resuming after a pop
- 🛠️ **Convenient macros** for easier transitions:

```cpp
  GSM_SWITCH(MenuState);
  GSM_PUSH(GameState);
  GSM_POP();
```

## 📂 Structure du projet

```bash
project/
│
├── src/
│   └── libs/
│   │   ├── common.h        # Global includes, shared variables
│   │   ├── gamestate.h     # Definition of the GameState structure
│   │   └── gamestate.c     # Implementation of the state manager
│   │
│   ├── states/
│   │   ├── state_menu.c    # Example state: menu
│   │   ├── state_game.c    # Example state: game
│   │   └── state_pause.c   # Example state: pause
│   └── main.c              # Entry point
│
├── Makefile
├── README_EN.md
└── README.md
```

## Example Usage

Declaring a state (Menu)
```cpp
#include <raylib.h>
#include "./libs/gamestate.h"

void Menu_Init(GameState *state) { /* load resources */ }
void Menu_Enter(GameState *state, GameState *from) { }
void Menu_Update(GameState *state, float dt) {
    if (IsKeyPressed(KEY_ENTER)) {
        GSM_SWITCH(GameState_Game); // switch to the game state
    }
}
void Menu_Draw(GameState *state) {
    DrawText("Main menu - press ENTER", 50, 50, 20, RAYWHITE);
}
void Menu_Leave(GameState *state) { /* cleanup */ }

GameState GameState_Menu = {
    .init = Menu_Init,
    .enter = Menu_Enter,
    .update = Menu_Update,
    .draw = Menu_Draw,
    .leave = Menu_Leave,
};
```
In `main.c`

```cpp
GamestateManager globalGSM;

int main(void) {
    InitWindow(800, 600, "Tiny State Manager - Raylib");
    SetTargetFPS(60);

    GSM_Init(&globalGSM);
    GSM_SwitchTo(&globalGSM, &GameState_Menu);

    while (!WindowShouldClose()) {
        GSM_Update(&globalGSM, GetFrameTime());
        BeginDrawing();
            ClearBackground(BLACK);
            GSM_Draw(&globalGSM);
        EndDrawing();
    }

    GSM_Free(&globalGSM);
    CloseWindow();
    return 0;
}
```

## 🗂️ Stack Behavior
The state manager works like a **stack** :
```scss
Initially:
[ MenuState ]   <- current state

After Push(GameState):
[ MenuState ]
[ GameState ]   <- current state

After Pop():
[ MenuState ]   <- current state
```
Et un **Switch** remplace directement l’état courant :

```scss
Before Switch(Menu -> Game):
[ MenuState ]   <- current state

After:
[ GameState ]   <- current state
```

## 🎯 Advantages

* **Short and readable** code
* No external dependencies → **only [Raylib](https://www.raylib.com/)**
* **Low-level compatible** (Dreamcast, homebrew consoles, embedded systems)
* Easily extensible with minimal changes

## 🕹️ Example States Included

* **MenuState** → simple text-based menu
* **GameState** → example with the famous Amiga Boing Ball in 3D

## 📜 License

This project is licensed under the MIT License.
You are free to use, modify, and distribute it.

*✍️ Written for low-level and retro dev enthusiasts, with ❤*
