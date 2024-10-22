

#include "ClothApplication.h"
#include "../Physics/Constants.h"
#include "iostream"


/**
 * 
 */
bool Application::IsRunning()
{
    return running;
}

/** 
 *  Setup function(executed once in the beginning of the simulation)
 */
void Application::Setup()
{
    std::cout << "Application Initialization \n";
}

/**
 * Setup override
 */
void Application::Setup(int clothWidth, int clothHeight, int clothSpacing)
{
    graphic = new Graphics();
    mouse = new Mouse();
    running = Graphics::OpenWindow();

    clothWidth /= clothSpacing;
    clothHeight /= clothSpacing;

    const int startX = graphic->Width() * 0.5f - clothWidth * clothSpacing * 0.5f;
    const int startY = graphic->Height() * 0.1f;

    cloth = new Cloth(clothWidth, clothHeight, clothSpacing, startX, startY);

    lastUpdateTime = SDL_GetTicks();
}

/** 
 * Input processing
 */
void Application::Input()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT: running = false; break;

            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)    running = false;
                if (event.key.keysym.sym == SDLK_UP)        pushForce.y = -50 * PhysicEngine::PIXELS_PER_METER;
                if (event.key.keysym.sym == SDLK_RIGHT)     pushForce.x = 50 * PhysicEngine::PIXELS_PER_METER;
                if (event.key.keysym.sym == SDLK_DOWN)      pushForce.y = 50 * PhysicEngine::PIXELS_PER_METER;
                if (event.key.keysym.sym == SDLK_LEFT)      pushForce.x = -50 * PhysicEngine::PIXELS_PER_METER;
                break;

            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_UP)        pushForce.y = PhysicEngine::ZERO;
                if (event.key.keysym.sym == SDLK_RIGHT)     pushForce.x = PhysicEngine::ZERO;
                if (event.key.keysym.sym == SDLK_DOWN)      pushForce.y = PhysicEngine::ZERO;
                if (event.key.keysym.sym == SDLK_LEFT)      pushForce.x = PhysicEngine::ZERO;
                break;

            case SDL_MOUSEMOTION:
                int a, n;
                mouseCursor.x = event.motion.x;
                a = mouseCursor.x;
                n = mouseCursor.y;
                mouseCursor.y = event.motion.y;
                mouse->UpdatePosition(a, n);
                break;

            case SDL_MOUSEBUTTONDOWN:
                int x, y;
                SDL_GetMouseState(&x, &y);
                mouse->UpdatePosition(x, y);

                if (!mouse->GetLeftButtonDown() && event.button.button == SDL_BUTTON_LEFT)
                {
                    mouse->SetLeftMouseButton(true);
                }
                if (!mouse->GetRightMouseButton() && event.button.button == SDL_BUTTON_RIGHT)
                {
                    mouse->SetRightMouseButton(true);
                }
                break;

            case SDL_MOUSEBUTTONUP:
                if (mouse->GetLeftButtonDown() && event.button.button == SDL_BUTTON_LEFT)
                {
                    mouse->SetLeftMouseButton(false);
                }
                if (mouse->GetRightMouseButton() && event.button.button == SDL_BUTTON_RIGHT)
                {
                    mouse->SetRightMouseButton(false);
                }
                break;

            case SDL_MOUSEWHEEL:
                if (event.wheel.y > PhysicEngine::ZERO)
                {
                    mouse->IncreaseCursorSize(10);
                }
                else if (event.wheel.y < PhysicEngine::ZERO)
                {
                    mouse->IncreaseCursorSize(-10);
                }
                break;

            default: break;
        }
    }
}

/**
 *   Update function(called several times per second to update objects)
 */
void Application::Update()
{
    const Uint32 currentTime = SDL_GetTicks();
    float deltaTime = (currentTime - lastUpdateTime) / PhysicEngine::DotPerTimeSeconds;

    cloth->Update(graphic, mouse, deltaTime);

    lastUpdateTime = currentTime;
}

/** 
 * Render function (called several times per second to draw objects)
 */
void Application::Render()
{
    Graphics::ClearScreen(0xFF00000000);

    cloth->Draw(graphic);

    Graphics::RenderFrame();
}

/**
 * Destroy function to delete objects and close the window
 */
void Application::Destroy()
{
    delete mouse;
    delete graphic;
    delete cloth;

    Graphics::CloseWindow();
}
