#pragma once

#include "../AngryApp/Graphics.h"
#include "Vector2D.h"
#include "./Objects/Stick.h"
#include "Mouse.h"

/**
 *
 */
class Point
{
private:
    Stick* sticks[2] = {nullptr};

    Vector2D pos;
    Vector2D prevPos;
    Vector2D initPos;
    bool isPinned = false;

    bool isSelected = false;

    void KeepInsideView(int windowWidth, int windowHeight);

public:
    Point() = default;
    Point(float x, float y);
    ~Point() = default;

    void AddStick(Stick* stick, int index);

    const Vector2D& GetPosition() const { return pos; }
    void SetPosition(float x, float y);

    void Pin();

    void Update(float deltaTime, float drag, const Vector2D& acceleration, float elasticity, Mouse* mouse, int windowWidth, int windowHeight);
};

