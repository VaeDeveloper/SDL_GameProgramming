#ifndef APPLICATION_H
#define APPLICATION_H

#include "../Physics/Graphics/Graphics.h"
#include <vector>
#include "../Physics/World.h"
#include "../Physics/Particle.h"

class Body;

/**
* @Discrioption: Application Class (
* @func     IsRunning();
* @func     Setup();
* @func     Input();
* @func     Update();
* @func     Render();
* @func     Destroy();
*/
class Application
{
private:
    bool running = false;
    std::vector<Particle*> particles;
    Vector2D pushForce = Vector2D(0, 0);
    Vector2D mouseCursor = Vector2D(0, 0);
    bool leftMouseButtonDown = false;

    Vector2D anchor;
    float k = 300;
    float restLength = 15;
    const int NUM_PARTICLES = 15;
        
public:
    Application() = default;
    ~Application() = default;
    bool IsRunning() const;
    void Setup();
    void Input();
    void Update();
    void Render();
    void RenderObjects();
    void Destroy();
    float TimeDeductions();
};
#endif
