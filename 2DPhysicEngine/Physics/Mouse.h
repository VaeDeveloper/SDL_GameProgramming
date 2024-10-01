#pragma once


#include "Vector2D.h"

/*---------------------------------------------------------------------------*/
class Mouse
{
private:
    Vector2D pos;
    Vector2D prevPos;

    float maxCursorSize = 150;
    float minCursorSize = 10;

    float cursorSize = 20;

    bool leftButtonDown = false;
    bool rightButtonDown = false;

public:
    Mouse() = default;
    ~Mouse() = default;

    const Vector2D& GetPosition() const { return pos; }
    const Vector2D& GetPreviousPosition() const { return prevPos; }
    void UpdatePosition(int x, int y);

    bool GetLeftButtonDown() const { return leftButtonDown; }
    void SetLeftMouseButton(bool state) { this->leftButtonDown = state; }

    bool GetRightMouseButton() const { return rightButtonDown; }
    void SetRightMouseButton(bool state) { this->rightButtonDown = state; }

    void IncreaseCursorSize(float increment);
    float GetCursorSize() const { return cursorSize; }
};
/*---------------------------------------------------------------------------*/
