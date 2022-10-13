#include <cstdio>
#include <cmath>
#include <chrono>

#include "demo_fractal.hpp"

// ##################################################### Square #####################################################

void DemoSquare::UpdateAndDraw()
{
    draw_fractal_squares(GetScreenWidth()/2, GetScreenHeight()/2, 300, 3, BLACK);
}

void DemoSquare::draw_fractal_squares(int centerX, int centerY, float sideLength, int depth, Color color)
{
    DrawRectangleLines(centerX - sideLength/2, centerY - sideLength/2, sideLength, sideLength, color);

    if(depth > 0)
    {
        depth--;
        if(depth == 2)
            color = BLUE;
        else if(depth == 1)
            color = RED;
        else if(depth == 0)
            color = GREEN;
            
        draw_fractal_squares(centerX - sideLength/2, centerY - sideLength/2, sideLength/2, depth, color);
        draw_fractal_squares(centerX + sideLength/2, centerY - sideLength/2, sideLength/2, depth, color);
        draw_fractal_squares(centerX - sideLength/2, centerY + sideLength/2, sideLength/2, depth, color);
        draw_fractal_squares(centerX + sideLength/2, centerY + sideLength/2, sideLength/2, depth, color);
    }
}

// ##################################################### Triangle #####################################################

void DemoTriangle::UpdateAndDraw()
{
    draw_sierpinski_triangle(
        GetScreenWidth()/2, GetScreenHeight()/2-300, 
        GetScreenWidth()/2-300, GetScreenHeight()/2+300, 
        GetScreenWidth()/2+300, GetScreenHeight()/2+300, 
        3, BLACK);
}


void DemoTriangle::draw_sierpinski_triangle(int x1, int y1, int x2, int y2, int x3, int y3, int depth, Color color)
{
    DrawTriangleLines((Vector2){(float)x1, (float)y1}, (Vector2){(float)x2, (float)y2}, (Vector2){(float)x3, (float)y3}, color);

    if(depth > 0)
    {
        depth--;
        draw_sierpinski_triangle(x1, y1, x1/2 + x2/2, y1/2 + y2/2, x1/2 + x3/2, y1/2 + y3/2,  depth, color);
        draw_sierpinski_triangle(x1/2 + x2/2, y1/2 + y2/2, x2, y2, x2/2 + x3/2, y2/2 + y3/2,  depth, color);
        draw_sierpinski_triangle(x1/2 + x3/2, y1/2 + y3/2, x2/2 + x3/2, y2/2 + y3/2, x3, y3, depth, color);
    }
}

// ##################################################### Polygon #####################################################

void DemoPolygon::UpdateAndDraw()
{
    draw_polygon_spiral(6, 300, 270, GetScreenWidth() / 2, GetScreenHeight() / 2, 10, 30, BLACK);
}

void DemoPolygon::DrawPolygonLines(int nGones, float radius, float angleOffset, float posOffsetX, float posOffsetY, Color color)
{
    angleOffset *= PI / 180;
    float angle = 2*PI / nGones;
    // DrawCircleLines(posOffsetX, posOffsetY, radius, color);
    for(int i = 0; i < nGones+1; i++)
    {
        DrawLine(
            posOffsetX + radius * cosf(angle * i + angleOffset), posOffsetY + radius * sinf(angle * i + angleOffset), 
            posOffsetX + radius * cosf(angle * i + angleOffset + angle),  posOffsetY + radius * sinf(angle * i + angleOffset + angle), 
            color);
    }
}

void DemoPolygon::draw_polygon_spiral(int nGones, float radius, float angleOffset, 
float posOffsetX, float posOffsetY,	float kReduction, int depth, Color color)
{
    DrawPolygonLines(nGones, radius, angleOffset, posOffsetX, posOffsetY, color);

    if(depth > 0)
    {
        depth--;
        radius -= kReduction;
        angleOffset += (cosf(kReduction*PI/180)*180/PI);
        draw_polygon_spiral(nGones, radius, angleOffset, posOffsetX, posOffsetY, kReduction, depth, color);
    }
}