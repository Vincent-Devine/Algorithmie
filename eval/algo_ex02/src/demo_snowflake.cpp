
#include <cmath>
#include <raylib.h>

#include "ui.hpp"
#include "demo_snowflake.hpp"

void DrawKochBranch(Vector2 p0, Vector2 p1, int depth)
{
    if(depth > 0)
    {
        float angle = PI/3;
        Vector2 p2 = (Vector2){(2*p0.x + p1.x)/3, (2*p0.y + p1.y)/3};
        Vector2 p4 = (Vector2){(2*p1.x + p0.x)/3, (2*p1.y + p0.y)/3};
        Vector2 p3 = (Vector2){p2.x + (p4.x - p2.x)*cosf(angle) + (p4.y - p2.y)*sinf(angle),
                               p2.y - (p4.x - p2.x)*sinf(angle) + (p4.y - p2.y)*cosf(angle)}; // Sommet nouveau triangle

        DrawKochBranch(p0, p2, depth-1);
        DrawKochBranch(p2, p3, depth-1);
        DrawKochBranch(p3, p4, depth-1);
        DrawKochBranch(p4, p1, depth-1);
    }
    else
        DrawLine(p0.x, p0.y, p1.x, p1.y, BLACK);
}

void DemoSnowflake::UpdateAndDraw()
{
    // TODO: Implémentez la fractale du flocon de neige
    int height = 200;

    float depthPointsFloat = depth;
    if (SliderFloat("Depth points", 2, 50, 200, 30, 0.f, 4.f, &depthPointsFloat))
        depth = (int)roundf(depthPointsFloat);

    Vector2 p0 = (Vector2){(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 - height};
    Vector2 p1 = (Vector2){(float)GetScreenWidth() / 2 - height, (float)GetScreenHeight() / 2 + height};
    Vector2 p2 = (Vector2){(float)GetScreenWidth() / 2 + height, (float)GetScreenHeight() / 2 + height};
    
    DrawKochBranch(p1, p0, depth);
    DrawKochBranch(p2, p1, depth);
    DrawKochBranch(p0, p2, depth);
}