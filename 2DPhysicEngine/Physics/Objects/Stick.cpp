
#include "Stick.h"
#include "../Point.h"

/*---------------------------------------------------------------------------*/
Stick::Stick(Point& p0, Point& p1, float lenght) : point0(p0), point1(p1), Length(lenght)
{
	// 
}
/*---------------------------------------------------------------------------*/
void Stick::Update()
{
    if (!bIsActive) return;

    Vector2D p0Pos = point0.GetPosition();
    Vector2D p1Pos = point1.GetPosition();

    Vector2D diff = p0Pos - p1Pos;
    float dist = sqrtf(diff.x * diff.x + diff.y * diff.y);
    float diffFactor = (Length - dist) / dist;
    Vector2D offset = diff * diffFactor * 0.5f;

    point0.SetPosition(p0Pos.x + offset.x, p0Pos.y + offset.y);
    point1.SetPosition(p1Pos.x - offset.x, p1Pos.y - offset.y);
}
/*---------------------------------------------------------------------------*/
void Stick::Draw(const Graphics* renderer) const
{
    if (!bIsActive) return;

    Vector2D p0Pos = point0.GetPosition();
    Vector2D p1Pos = point1.GetPosition();

    renderer->DrawLine(p0Pos.x, p0Pos.y, p1Pos.x, p1Pos.y, bIsSelected ? colorWhenSelected : color);
}
/*---------------------------------------------------------------------------*/
void Stick::Break()
{
    bIsActive = false;
}
/*---------------------------------------------------------------------------*/
void Stick::SetIsSelected(bool value)
{
    bIsSelected = value;
}
/*---------------------------------------------------------------------------*/
