#include <math.h>
#include "../libs/gamestate.h"

#define DEG2RAD (PI/180.0f)

// Game Variables
static Camera camera = {0};

static Vector3 ballPos = {0};
static Vector3 ballVelocity = {0};
static float ballRadius = 1.4f;
static float ballHeight = 1.8f;

static int ballSlices = 16; // number of segments horizontally
static int ballStacks = 8; // number of segments vertically

// Room boundaries
static float floorY = 0.0f;
static float leftX = -3.5f, rightX = 3.5f;

static float gravity = -6.0f;           // Gravity acceleration
static float horizontalSpeed = 1.0f;    // Horizontal speed of the ball

static float ballRotationY = 0.0f;      // Rotation around Y axis
static float rotationSpeed = 200.0f;    // Degrees per second
static Vector3 rotationAxis = {0};

void DrawBoingBall(Vector3 center, float radius, int slices, int stacks, float rotationY, float tiltAngle);
void DrawHorizontalGrid(Vector3 center, int rows, int cols, float spacing, Color color);
void DrawVerticalGrid(Vector3 center, int rows, int cols, float spacing, Color color);

static void Game_Init(void) {
	ballPos = (Vector3){leftX, ballRadius, 3.01f};                                          // start at left side
    ballVelocity = (Vector3){horizontalSpeed, sqrtf(2.0f * -gravity * ballHeight), 0.0f};   // initial velocity

	rotationAxis = (Vector3){1.0f, sinf(DEG2RAD*30.0f), 0.0f};

	camera.position = (Vector3){0.0f, 2.0f, 10.0f};
    camera.target = (Vector3){0.0f, 2.0f, 0.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
}

static void Game_Enter(void) { TraceLog(LOG_INFO, "Game entered!"); }
static void Game_Leave(void) { TraceLog(LOG_INFO, "Game stopped, freeing resources"); }
static void Game_Resume(void) { TraceLog(LOG_INFO, "Game resumed!"); }

static void Game_Update(float dt) {

	// Update ball position
	ballPos.x += ballVelocity.x * dt;

    // Limits left/right
    if (ballPos.x - ballRadius < leftX) {
        ballPos.x = leftX + ballRadius;
        ballVelocity.x *= -1.0f;
		rotationSpeed *= -1;
    }

    if (ballPos.x + ballRadius > rightX) {
        ballPos.x = rightX - ballRadius;
        ballVelocity.x *= -1.0f;
		rotationSpeed *= -1;
    }

	// Vertical movement
    ballVelocity.y += gravity * dt;     // Gravity affects vertical velocity
    ballPos.y += ballVelocity.y * dt;

    // Rebond sur le sol
    if (ballPos.y - ballRadius < floorY) {
        ballPos.y = floorY + ballRadius;

        // Given the desired jump height (h) and gravity (g), the initial velocity (v) can be calculated using the formula: v = sqrt(2 * g * h)
        ballVelocity.y = sqrtf(2.0f * -gravity * ballHeight); // 2.0f = hauteur désirée
    }

	// Vertical rotation
	ballRotationY += rotationSpeed * dt;
	if(ballRotationY >= 360.0f) ballRotationY -= 360.0f;
    if(ballRotationY < 0.0f) ballRotationY += 360.0f;

    if (IsKeyPressed(KEY_P)) {
        GS_PUSH(PauseState); // Push PauseState on top of GameState
    }

	if (IsKeyPressed(KEY_ENTER)) {
        GS_SWITCH(MenuState); // Switch from GameState → to MenuState
    }
}

static void Game_Draw(void) {
    ClearBackground((Color){ 170, 170, 170, 255 });

	BeginMode3D(camera);
	{
		// Draw room
		DrawVerticalGrid((Vector3){0,2.6f,3}, 10, 16, 0.5f, (Color){ 163, 24, 161, 255 });
		DrawHorizontalGrid((Vector3){0,0,4.2f}, 10, 16, 0.6f, (Color){ 163, 24, 161, 255 });

		// boing ball
		DrawBoingBall(ballPos, ballRadius, ballSlices, ballStacks, ballRotationY, 30.0f);
	}
	EndMode3D();

    DrawText("GAME STATE", 240, 20, 20, WHITE);
    DrawText("Press P to pause", 220, 440, 20, YELLOW);
	DrawText("Press ENTER to Menu", 190, 460, 20, BLUE);
}

void DrawBoingBall(Vector3 center, float radius, int slices, int stacks, float rotationY, float tiltAngle) {
    float cosTilt = cosf(DEG2RAD * tiltAngle);
    float sinTilt = sinf(DEG2RAD * tiltAngle);

    for(int i = 0; i < stacks; i++) {
        float lat0 = PI * (-0.5f + (float)i / stacks);
        float lat1 = PI * (-0.5f + (float)(i+1) / stacks);
        float y0 = radius * sinf(lat0);
        float y1 = radius * sinf(lat1);
        float r0 = radius * cosf(lat0);
        float r1 = radius * cosf(lat1);

        for(int j = 0; j < slices; j++) {
            float lng0 = 2*PI * j / slices;
            float lng1 = 2*PI * (j+1) / slices;

            Vector3 v0 = {r0*cosf(lng0), y0, r0*sinf(lng0)};
            Vector3 v1 = {r1*cosf(lng0), y1, r1*sinf(lng0)};
            Vector3 v2 = {r1*cosf(lng1), y1, r1*sinf(lng1)};
            Vector3 v3 = {r0*cosf(lng1), y0, r0*sinf(lng1)};

            // Rotation autour de Y pur
            for(int k=0;k<4;k++){
                Vector3 *v = (k==0?&v0:(k==1?&v1:(k==2?&v2:&v3)));

                float x = v->x*cosf(DEG2RAD*rotationY) + v->z*sinf(DEG2RAD*rotationY);
                float z = -v->x*sinf(DEG2RAD*rotationY) + v->z*cosf(DEG2RAD*rotationY);
                v->x = x;
                v->z = z;

                // Inclinaison fixe autour X
                float y = v->y*cosTilt - v->z*sinTilt;
                z       = v->y*sinTilt + v->z*cosTilt;
                v->y = y;
                v->z = z;

                // Translation vers le centre
                v->x += center.x;
                v->y += center.y;
                v->z += center.z;
            }

            Color color = ((i+j)%2==0) ? RED : WHITE;
            DrawTriangle3D(v0,v1,v2,color);
            DrawTriangle3D(v0,v2,v3,color);
        }
    }
}


void DrawHorizontalGrid(Vector3 center, int rows, int cols, float spacing, Color color) {
    float halfWidth  = (cols - 1) * 0.5f * spacing;
    float halfDepth  = (rows - 1) * 0.5f * spacing;

    // Lignes “X” (de gauche à droite)
    for(int c = 0; c < cols; c++) {
        float x = center.x + c * spacing - halfWidth;
        Vector3 start = {x, center.y, center.z - halfDepth};
        Vector3 end   = {x, center.y, center.z + halfDepth};
        DrawLine3D(start, end, color);
    }

    // Lignes “Z” (de devant à derrière)
    for(int r = 0; r < rows; r++) {
        float z = center.z + r * spacing - halfDepth;
        Vector3 start = {center.x - halfWidth, center.y, z};
        Vector3 end   = {center.x + halfWidth, center.y, z};
        DrawLine3D(start, end, color);
    }
}

void DrawVerticalGrid(Vector3 center, int rows, int cols, float spacing, Color color) {
    float halfWidth  = (cols - 1) * 0.5f * spacing;
    float halfHeight = (rows - 1) * 0.5f * spacing;

    // Lignes verticales (de haut en bas)
    for(int c = 0; c < cols; c++) {
        float x = center.x + c * spacing - halfWidth;
        Vector3 start = {x, center.y + halfHeight, center.z};
        Vector3 end   = {x, center.y - halfHeight, center.z};
        DrawLine3D(start, end, color);
    }

    // Lignes horizontales (de gauche à droite)
    for(int r = 0; r < rows; r++) {
        float y = center.y + r * spacing - halfHeight;
        Vector3 start = {center.x - halfWidth, y, center.z};
        Vector3 end   = {center.x + halfWidth, y, center.z};
        DrawLine3D(start, end, color);
    }
}

STATE_DEFINE(GameState, Game_Init, Game_Enter, Game_Leave, Game_Resume, Game_Update, Game_Draw);
