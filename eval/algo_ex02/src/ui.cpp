#include <raylib.h>

#include "common.hpp"
#include "calc.hpp"

inline bool InRect(int x, int y, int rectX, int rectY, int width, int height)
{
    if (x < rectX)          return false;
    if (x > rectX + width)  return false;
    if (y < rectY)          return false;
    if (y > rectY + height) return false;
    return true;
}

bool Button(int x, int y, int width, const char* label)
{
    int fontSize      = GetFontDefault().baseSize * 2;
    int height        = fontSize * 2;
    bool isHover      = InRect(GetMousePosition().x, GetMousePosition().y, x, y, width, height);
    bool mouseDown    = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    bool mousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    DrawRectangle(x, y, width, height, (isHover && mouseDown) ? LIGHTGRAY : GRAY);
    DrawText(label, x + fontSize / 2.f, y + fontSize / 2.f, fontSize, WHITE);

    return isHover && mousePressed;
}

bool SliderFloat(const char* label, int x, int y, int width, int height, float min, float max, float* value)
{
    bool hasChanged = false;

    // Draw green bar
    float ratio = calc::inverseLerp(min, max, *value); // same as : float ratio = (*value - min) / (max - min);
    DrawRectangle(x, y, width * ratio, height, GREEN);

    // Draw borders
    DrawRectangleLines(x, y, width, height, BLACK);

    // Compute new value if mouse is down inside the rect
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
    {
        int mouseX = GetMousePosition().x;
        int mouseY = GetMousePosition().y;
        if (InRect(mouseX, mouseY, x, y, width, height))
        {
            float ratio = (mouseX - x) / (float)width;
            *value = calc::lerp(min, max, ratio);
            hasChanged = true;
        }
    }

    // Draw text (label + value)
    DrawText(TextFormat("%s: %.2f", label, *value), x + 10, y + 5, GetFontDefault().baseSize * 2, BLACK);

    return hasChanged;
}

bool Checkbox(const char* label, int x, int y, bool* value)
{
    int size = 16;
    if (*value)
        DrawRectangle(x, y, size, size, BLACK);
    else
        DrawRectangleLines(x, y, size, size, BLACK);

    bool changed = false;
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && InRect(GetMouseX(), GetMouseY(), x, y, size + 100, size))
    {
        *value = !*value;
        changed = true;
    }

    DrawText(label, x + size + 5, y, GetFontDefault().baseSize * 2, BLACK);

    return changed;
}
