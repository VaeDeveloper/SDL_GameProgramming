#pragma once

#include "../Graphics/Graphics.h"

class Point;
class Stick
{
public:
    Stick(Point& p0, Point& p1, float lenght, Uint32 color, Uint32 selectedColor);
    Stick(Point& p0, Point& p1, float lenght);
    ~Stick() = default;

    void SetIsSelected(bool value);

    void Update();
    void Draw(const Graphics* renderer) const;
    void Break();

private:
    Point& point0;
    Point& point1;

    float Length;

    bool bIsActive = true;
    bool bIsSelected = false;

    Uint32 color = 0xFF00FF00;
    Uint32 colorWhenSelected = 0xFFFF0000;
};
