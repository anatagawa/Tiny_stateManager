#include <math.h>
#include "../libs/gamestate.h"

// Menu Variables
static Camera camera = {0};

static Vector3 pyramidVertices[5] = {0}; // 4 base + 1 sommet
static int pyramidEdges[8][2] = {0};
static float rotationX = 0.0f;
static float rotationY = 0.0f;
static float rotationZ = 0.0f;
static Vector3 rotated[5] = {0};

Vector3 RotateVector(Vector3 v, float rx, float ry, float rz);

// Callbacks
static void Menu_Init(void) {
    pyramidVertices[0] = (Vector3){-1, 0, -1};
    pyramidVertices[1] = (Vector3){1, 0, -1};
    pyramidVertices[2] = (Vector3){1, 0, 1};
    pyramidVertices[3] = (Vector3){-1, 0, 1};

    pyramidVertices[4] = (Vector3){0, 2, 0}; // sommet

    int edges[8][2] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0}, // base
        {0, 4}, {1, 4}, {2, 4}, {3, 4}  // sides
    };

    for (int i = 0; i < 8; i++) {
        pyramidEdges[i][0] = edges[i][0];
        pyramidEdges[i][1] = edges[i][1];
    }

    camera.position = (Vector3){5.0f, 0.0f, 0.0f};
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
}

static void Menu_Enter(void) { TraceLog(LOG_INFO, "Menu entered!"); }
static void Menu_Leave(void) { TraceLog(LOG_INFO, "Menu left, freeing resources"); }
static void Menu_Resume(void) { TraceLog(LOG_INFO, "Menu resumed!"); }

static void Menu_Update(float dt) {
    rotationX += 30.0f * dt; // degrés/sec
    rotationY += 45.0f * dt;
    rotationZ += 60.0f * dt;

    if (rotationX >= 360) rotationX -= 360;
    if (rotationY >= 360) rotationY -= 360;
    if (rotationZ >= 360) rotationZ -= 360;

    for(int i=0;i<5;i++) {
        rotated[i] = RotateVector(pyramidVertices[i], rotationX, rotationY, rotationZ);
    }

    if (IsKeyPressed(KEY_ENTER)) {
        GS_SWITCH(GameState); // Switch from MenuState → to GameState
    }
}

static void Menu_Draw(void) {
    ClearBackground(RAYWHITE);
    DrawText("MENU STATE", 240, 100, 20, BLACK);
    DrawText("Press ENTER to start", 190, 130, 20, RED);

    BeginMode3D(camera);
    {
        // Draw pyramid edges
        for (int i = 0; i < 8; i++) {
            DrawLine3D(rotated[pyramidEdges[i][0]], rotated[pyramidEdges[i][1]], BLACK);
        }
    }
    EndMode3D();
}

Vector3 RotateVector(Vector3 v, float rx, float ry, float rz) {
    Vector3 r = v;

    // Rotation X
    float radX = DEG2RAD * rx;
    float cosX = cosf(radX);
    float sinX = sinf(radX);
    float y = r.y * cosX - r.z * sinX;
    float z = r.y * sinX + r.z * cosX;
    r.y = y; r.z = z;

    // Rotation Y
    float radY = DEG2RAD * ry;
    float cosY = cosf(radY);
    float sinY = sinf(radY);
    float x = r.x * cosY + r.z * sinY;
    z = -r.x * sinY + r.z * cosY;
    r.x = x; r.z = z;

    // Rotation Z
    float radZ = DEG2RAD * rz;
    float cosZ = cosf(radZ);
    float sinZ = sinf(radZ);
    x = r.x * cosZ - r.y * sinZ;
    y = r.x * sinZ + r.y * cosZ;
    r.x = x; r.y = y;

    return r;
}

STATE_DEFINE(MenuState, Menu_Init, Menu_Enter, Menu_Leave, Menu_Resume, Menu_Update, Menu_Draw);
