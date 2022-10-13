#pragma once

#include "demo.hpp"
#include <raylib.h>

// ##################################################### Square #####################################################

class DemoSquare : public Demo
{
public:
    const char* GetName() override { return "Fractal square";}
    void UpdateAndDraw() override;

private:
    void draw_fractal_squares(
        int centerX, int centerY, 
        float sideLength, 
        int depth, Color color);
};

// ##################################################### Triangle #####################################################

class DemoTriangle : public Demo
{
public:
    const char* GetName() override { return "Fractal triangle";}
    void UpdateAndDraw() override;

private:
    void draw_sierpinski_triangle(
        int x1, int y1, 
        int x2, int y2, 
        int x3, int y3, 
        int depth, Color color);
};

// ##################################################### Polygon #####################################################

class DemoPolygon : public Demo
{
public:
    const char* GetName() override { return "Fractal polygon";}
    void UpdateAndDraw() override;

private:
    void DrawPolygonLines(
        int nGones, float radius, 
        float angleOffset, float posOffsetX, float posOffsetY, 
        Color color);

    void draw_polygon_spiral(
        int nGones, float radius, 
        float angleOffset, float posOffsetX, float posOffsetY,	
        float kReduction, 
        int depth, Color color);
};