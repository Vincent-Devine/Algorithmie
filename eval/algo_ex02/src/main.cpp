#include <raylib.h>

#include "demo.hpp"
#include "demo_snowflake.hpp"

int main(void)
{
    const int screenWidth  = 1280;
    const int screenHeight = 800;

    SetConfigFlags(FLAG_VSYNC_HINT);
    //SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "2D algorithm demos");

    Demo* demoList[] = {
        new DemoSnowflake(),
    };

    const int count = sizeof(demoList) / sizeof(Demo*);
    int current = 0;

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        if (IsKeyPressed(KEY_LEFT))
            current -= 1;
        if (IsKeyPressed(KEY_RIGHT))
            current += 1;
        
        current = current % count;
        if (current < 0)
            current += count;

        demoList[current]->UpdateAndDraw();
        
        DrawText(TextFormat("%d/%d : %s", current+1, count, demoList[current]->GetName()), screenWidth / 2 - 50, 10, 20, BLACK);
        DrawFPS(10, 10);

        EndDrawing();
    }

    for (Demo* demo : demoList)
        delete demo;

    CloseWindow();

    return 0;
}