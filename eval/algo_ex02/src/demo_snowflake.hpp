#pragma once

#include "demo.hpp"

class DemoSnowflake : public Demo
{
public:
    const char* GetName() override { return "Snowflake"; }
    void UpdateAndDraw() override;

private:
    int depth = 0;
};