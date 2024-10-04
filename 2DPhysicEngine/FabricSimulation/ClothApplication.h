#ifndef CLOTHAPPLICATION_H
#define CLOTHAPPLICATION_H


#include "./Graphics.h"
#include <vector>
#include "../Physics/Objects/Cloth.h"
#include "../Physics/Mouse.h"

/*------------------------------------------------------------------------------------------------------*/
class Application
{
private:
    bool running = false;
    bool leftMouseButtonDown = false;
    bool rightMouseButtonDown = false;

    float k = 1500;
    float restLength = 200;
    const int NUM_BodyS = 4;


    Vector2D pushForce = Vector2D(0.0, 0.0);
    Vector2D mouseCursor = Vector2D(0, 0);
    Vector2D anchor;

    SDL_Rect liquid;

    Graphics* graphic = nullptr;
    Mouse* mouse = nullptr;
    Cloth* cloth = nullptr;

    Uint32 lastUpdateTime;

public:
    Application() = default;
    virtual ~Application() = default;
    bool IsRunning();
    void Setup();

    void Setup(int clothWidth, int clothHeight, int clothSpacing);
    void Input();
    void Update();
    void Render();
    void Destroy();
};
/*------------------------------------------------------------------------------------------------------*/
#endif
