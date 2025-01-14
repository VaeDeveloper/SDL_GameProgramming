#ifndef APPLICATION_H
#define APPLICATION_H

#include "../Physics/Graphics/Graphics.h"
#include "../Physics/Particle.h"
#include "../Physics/World.h"
#include <vector>

class Application 
{
    bool running = false;
    bool debug = false;
    bool leftMouseButtonDown = false;
    float k = 1500;
    float restLength = 200;
    const int NUM_PARTICLES = 4;

    Vector2D pushForce = Vector2D(0, 0);
    Vector2D mouseCursor = Vector2D(0, 0);

    std::vector<Particle*> particles;
    World* world = nullptr;

public:
    Application() = default;
    ~Application() = default;
    bool IsRunning();
    void Setup();
    void Input();
    void Update();
    void Render();
    void Destroy();
};

#endif
