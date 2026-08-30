#include "game.h"
#include <raylib.h>

// Configura Raylib y mantiene un ciclo principal pequeño que delega en Game.
int main(int argc, char **)
{
    const bool smokeTest = argc > 1;
    unsigned int flags = FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT;
    if (smokeTest)
        flags |= FLAG_WINDOW_HIDDEN;
    SetConfigFlags(flags);
    InitWindow(1280, 720, "Red Shift Tetris");
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);

    {
        Game game(smokeTest);
        while (!WindowShouldClose() && !game.IsSmokeTestFinished())
        {
            game.Update();
            game.Draw();
        }
    }

    CloseWindow();
    return 0;
}
