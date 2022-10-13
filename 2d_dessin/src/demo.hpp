#pragma once

class Demo
{
public:
    virtual const char* GetName() { return "Unnamed"; }
    virtual ~Demo() {}
    virtual void UpdateAndDraw() = 0;
};