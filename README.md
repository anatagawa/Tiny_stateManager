# 🎮 Tiny State Manager en C avec [Raylib](https://www.raylib.com/)

Un petit **gestionnaire de Game States** (états de jeu) écrit en **C pur** pour Raylib, pensé pour la **compatibilité maximale** (PC, Dreamcast, consoles rétro).  
Il est minimaliste, lisible et extensible : idéal pour vos projets de jeux, démos, ou prototypes.


## 🚀 Fonctionnalités

- ⚡ **C pur** (pas de C++, pas de classes → portable partout)
- 🧩 **États modulaires** (menu, jeu, pause, etc.)
- 📦 **Gestion des transitions** :
  - `GSM_SwitchTo()` → changer d’état  
  - `GSM_Push()` → empiler un nouvel état  
  - `GSM_Pop()` → revenir en arrière
- 🔄 **Callbacks standardisés** pour chaque état :
  - `init` → initialisation (chargement ressources)
  - `enter` → entrée dans l’état
  - `update` → logique du jeu
  - `draw` → rendu graphique
  - `leave` → sortie de l’état (libération mémoire)
  - `resume` → reprise après un pop
- 🛠️ **Macros pratiques** pour simplifier les transitions :

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
│	└── libs/
│	│   ├── common.h		# Includes globaux, variables partagées
│	│   ├── gamestate.h		# Définition de la structure GameState
│	│   └── gamestate.c		# Implémentation du gestionnaire d’états
│	│
│	├── states/
│	│   ├── state_menu.c	# Exemple d’état : menu
│	│	├── state_game.c	# Exemple d’état : game
│	│   └── state_pause.c	# Exemple d’état : pause
│   └── main.c				# Point d’entrée
│
├── Makefile
└── README.md
```

## Exemple d'utilisation

Déclaration d’un état (Menu)
```cpp
#include <raylib.h>
#include "./libs/gamestate.h"

void Menu_Init(GameState *state) { /* charger ressources */ }
void Menu_Enter(GameState *state, GameState *from) { }
void Menu_Update(GameState *state, float dt) {
    if (IsKeyPressed(KEY_ENTER)) {
        GSM_SWITCH(GameState_Game); // passe au jeu
    }
}
void Menu_Draw(GameState *state) {
    DrawText("Menu principal - appuyez sur ENTREE", 50, 50, 20, RAYWHITE);
}
void Menu_Leave(GameState *state) { /* nettoyer */ }

GameState GameState_Menu = {
    .init = Menu_Init,
    .enter = Menu_Enter,
    .update = Menu_Update,
    .draw = Menu_Draw,
    .leave = Menu_Leave,
};
```
Dans `main.c`

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

## 🗂️ Fonctionnement du Stack
Le gestionnaire fonctionne comme une **pile (stack)** :
```scss
Au départ :
[ MenuState ]   <- état courant

Après un Push(GameState) :
[ MenuState ]
[ GameState ]   <- état courant

Après un Pop() :
[ MenuState ]   <- état courant
```
Et un **Switch** remplace directement l’état courant :

```scss
Avant Switch(Menu -> Game) :
[ MenuState ]   <- état courant

Après :
[ GameState ]   <- état courant
```

## 🎯 Avantages

* Code **court et lisible**
* Aucune dépendance externe → seulement **[Raylib](https://www.raylib.com/)**
* Compatible **bas niveau** (Dreamcast, consoles homebrew, systèmes embarqués)
* Extensible avec très peu de modifications

## 🕹️ Exemple inclus

* **MenuState** → menu simple avec texte
* **GameState** → exemple avec la célèbre Boing Ball Amiga en 3D

## 📜 Licence

Ce projet est fourni sous licence MIT.
Vous êtes libre de l’utiliser, le modifier, et le distribuer.

*✍️ Écrit pour les passionnés de dev bas-niveau et de rétro, avec ❤*