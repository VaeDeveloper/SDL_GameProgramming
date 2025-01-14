
#include "Cloth.h"
#include "../Constants.h"

Cloth::Cloth(int width, int height, int spacing, int startX, int startY)
{
    for (int y = PhysicEngine::ZERO; y <= height; y++)
    {
        for (int x = PhysicEngine::ZERO; x <= width; x++)
        {
            Point* point = new Point(startX + x * spacing, startY + y * spacing);

            if (x != PhysicEngine::ZERO)
            {
                Point* leftPoint = points[this->points.size() - PhysicEngine::POSITIVE];
                Stick* s = new Stick(*point, *leftPoint, spacing);
                leftPoint->AddStick(s, PhysicEngine::ZERO);
                point->AddStick(s, PhysicEngine::ZERO);
                sticks.push_back(s);
            }

            if (y != PhysicEngine::ZERO)
            {
                Point* upPoint = points[x + (y - PhysicEngine::POSITIVE) * (width + PhysicEngine::POSITIVE)];
                Stick* s = new Stick(*point, *upPoint, spacing);
                upPoint->AddStick(s, PhysicEngine::POSITIVE);
                point->AddStick(s, PhysicEngine::POSITIVE);
                sticks.push_back(s);
            }

            if (y == PhysicEngine::ZERO && x % 2 == PhysicEngine::ZERO)
            {
                point->Pin();
            }

            points.push_back(point);
        }
    }
}

Cloth::~Cloth()
{
    for (auto point : points) delete point;
    for (auto stick : sticks) delete stick;
}

void Cloth::Update(Graphics* renderer, Mouse* mouse, float deltaTime)
{
    for (int i = PhysicEngine::ZERO; i < static_cast<int>(points.size()); i++)
    {
        Point* p = points[i];
        p->Update(deltaTime, drag, gravity, elasticity, mouse, renderer->Width(), renderer->Height());
    };

    for (int i = PhysicEngine::ZERO; i < static_cast<int>(sticks.size()); i++)
    {
        sticks[i]->Update();
    };
}

void Cloth::Draw(Graphics* renderer) const
{
    for (int i = PhysicEngine::ZERO; i < static_cast<int>(sticks.size()); i++)
    {
        sticks[i]->Draw(renderer);
    }
}
