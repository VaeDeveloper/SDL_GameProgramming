

#include "ClothApplication.h"


void LoopingApp(Application& app)
{
    while (app.IsRunning())
    {
        app.Input();
        app.Update();
        app.Render();
    }
}

int main(int argc, char* args[])
{
    Application app;

    app.Setup(1000, 350, 10);

    app.Setup();
    LoopingApp(app);
    app.Destroy();

    return 0;
}