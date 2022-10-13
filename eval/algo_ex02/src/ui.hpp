#pragma once

bool Button(int x, int y, int width, const char* label);
bool SliderFloat(const char* name, int x, int y, int width, int height, float min, float max, float* value);
bool Checkbox(const char* label, int x, int y, bool* value);