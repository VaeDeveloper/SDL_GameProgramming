#ifndef APPLICATION_H
#define APPLICATION_H

#include "Graphics.h"
#include <vector>
#include "../Physics/World.h"

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
    bool debug = false;
    bool running = false;

    World* world = nullptr;

    SDL_Texture* bgTexture;
    Body* bob = nullptr;

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
/*------------------------------------------------------------------------------------------------------*/
#endif
