#include "Application.h"
#include "../Physics/Constants.h"
#include "../Physics/Force.h"
#include "../Physics/CollisionDetection.h"
#include "../Physics/Contact.h"
#include "../Physics/Constraint.h"

bool Application::IsRunning() const
{
    return running;
}
/*Setup function (executed once in the beginning of the simulation)*/

void Application::Setup()
{
    running = Graphics::OpenWindow();

    // Create a physics world with gravity of -9.8 m/s2
    world = new World(-9.8);
    
        // Create a physics world with gravity of -9.8 m/s2
    world = new World(-9.8);

    // Load texture for the background image
    SDL_Surface* bgSurface = IMG_Load("./assets/angrybirds/background.png");
    if (bgSurface)
    {
        //bgTexture = SDL_CreateTextureFromSurface(Graphics::Renderer, bgSurface);
        SDL_FreeSurface(bgSurface);
    }

    // Add bird
    Body* bird = new Body(CircleShape(45), 100, Graphics::Height() / 2.0 + 220, 3.0);
    bird->SetTexture("./assets/angrybirds/bird-red.png");
    world->AddBody(bird);

    // Add a floor and walls to contain objects objects
    Body* floor = new Body(BoxShape(Graphics::Width() - 50, 50), Graphics::Width() / 2.0, Graphics::Height() / 2.0 + 340, 0.0);
    Body* leftFence = new Body(BoxShape(50, Graphics::Height() - 200), 0, Graphics::Height() / 2.0 - 35, 0.0);
    Body* rightFence = new Body(BoxShape(50, Graphics::Height() - 200), Graphics::Width(), Graphics::Height() / 2.0 - 35, 0.0);
    Body* top = new Body(BoxShape(Graphics::Width() - 50, 50), Graphics::Width() / 2.0, Graphics::Height() / 2.0 + 720, 0.0);

    world->AddBody(floor);
    world->AddBody(leftFence);
    world->AddBody(rightFence);
    world->AddBody(top);

    // Add a stack of boxes
    for (int i = 1; i <= 4; i++)
    {
        float mass = 10.0 / (float)i;
        Body* box = new Body(BoxShape(50, 50), 600, floor->position.y - i * 55, mass);
        box->SetTexture("./assets/angrybirds/wood-box.png");
        box->friction = 0.9;
        box->restitution = 0.1;
        world->AddBody(box);
    }

    // Add structure with blocks
    Body* plank1 = new Body(BoxShape(50, 150), Graphics::Width() / 2.0 + 20, floor->position.y - 100, 5.0);
    Body* plank2 = new Body(BoxShape(50, 150), Graphics::Width() / 2.0 + 180, floor->position.y - 100, 5.0);
    Body* plank3 = new Body(BoxShape(250, 25), Graphics::Width() / 2.0 + 100.0f, floor->position.y - 200, 2.0);
    plank1->SetTexture("./assets/angrybirds/wood-plank-solid.png");
    plank2->SetTexture("./assets/angrybirds/wood-plank-solid.png");
    plank3->SetTexture("./assets/angrybirds/wood-plank-cracked.png");
    world->AddBody(plank1);
    world->AddBody(plank2);
    world->AddBody(plank3);

    // Add a triangle polygon
    std::vector<Vector2D> triangleVertices = {Vector2D(30, 30), Vector2D(-30, 30), Vector2D(0, -30)};
    Body* triangle = new Body(PolygonShape(triangleVertices), plank3->position.x, plank3->position.y - 50, 0.5);
    triangle->SetTexture("./assets/angrybirds/wood-triangle.png");
    world->AddBody(triangle);

    // Add a pyramid of boxes
    int numRows = 5;
    for (int col = 0; col < numRows; col++)
    {
        for (int row = 0; row < col; row++)
        {
            float x = (plank3->position.x + 200.0f) + col * 50.0f - (row * 25.0f);
            float y = (floor->position.y - 50.0f) - row * 52.0f;
            float mass = (5.0f / (row + 1.0f));
            Body* box = new Body(BoxShape(50, 50), x, y, mass);
            box->friction = 0.9;
            box->restitution = 0.0;
            box->SetTexture("./assets/angrybirds/wood-box.png");
            world->AddBody(box);
        }
    }

    // Add a bridge of connected steps and joints
    int numSteps = 10;
    int spacing = 33;
    Body* startStep = new Body(BoxShape(80, 20), 200, 200, 0.0);
    startStep->SetTexture("./assets/angrybirds/rock-bridge-anchor.png");
    world->AddBody(startStep);
    Body* last = floor;
    for (int i = 1; i <= numSteps; i++)
    {
        float x = startStep->position.x + 30 + (i * spacing);
        float y = startStep->position.y + 20;
        float mass = (i == numSteps) ? 0.0 : 3.0;
        Body* step = new Body(CircleShape(15), x, y, mass);
        step->SetTexture("./assets/angrybirds/wood-bridge-step.png");
        world->AddBody(step);
        JointConstraint* joint = new JointConstraint(last, step, step->position);
        world->AddConstraint(joint);
        last = step;
    }
    Body* endStep = new Body(BoxShape(80, 20), last->position.x + 60, last->position.y - 20, 0.0);
    endStep->SetTexture("./assets/angrybirds/rock-bridge-anchor.png");
    world->AddBody(endStep);

    // Add pigs
    Body* pig1 = new Body(CircleShape(30), plank1->position.x + 80, floor->position.y - 50, 3.0);
    Body* pig2 = new Body(CircleShape(30), plank2->position.x + 400, floor->position.y - 50, 3.0);
    Body* pig3 = new Body(CircleShape(30), plank2->position.x + 460, floor->position.y - 50, 3.0);
    Body* pig4 = new Body(CircleShape(30), 220, 130, 1.0);
    pig1->SetTexture("./assets/angrybirds/pig-1.png");
    pig2->SetTexture("./assets/angrybirds/pig-2.png");
    pig3->SetTexture("./assets/angrybirds/pig-1.png");
    pig4->SetTexture("./assets/angrybirds/pig-2.png");
    world->AddBody(pig1);
    world->AddBody(pig2);
    world->AddBody(pig3);
    world->AddBody(pig4);


    //// Add ragdoll parts (rigid bodies)
    bob = new Body(CircleShape(5), Graphics::Width() / 2.0, Graphics::Height() / 2.0 - 200, 0.0);
    Body* head = new Body(CircleShape(25), bob->position.x, bob->position.y + 70, 5.0);
    Body* torso = new Body(BoxShape(50, 100), head->position.x, head->position.y + 80, 3.0);
    Body* leftArm = new Body(BoxShape(15, 70), torso->position.x - 32, torso->position.y - 10, 1.0);
    Body* rightArm = new Body(BoxShape(15, 70), torso->position.x + 32, torso->position.y - 10, 1.0);
    Body* leftLeg = new Body(BoxShape(20, 90), torso->position.x - 20, torso->position.y + 97, 1.0);
    Body* rightLeg = new Body(BoxShape(20, 90), torso->position.x + 20, torso->position.y + 97, 1.0);
    bob->SetTexture("./assets/ragdoll/bob.png");
    head->SetTexture("./assets/ragdoll/head.png");
    torso->SetTexture("./assets/ragdoll/torso.png");
    leftArm->SetTexture("./assets/ragdoll/leftArm.png");
    rightArm->SetTexture("./assets/ragdoll/rightArm.png");
    leftLeg->SetTexture("./assets/ragdoll/leftLeg.png");
    rightLeg->SetTexture("./assets/ragdoll/rightLeg.png");
    world->AddBody(bob);
    world->AddBody(head);
    world->AddBody(torso);
    world->AddBody(leftArm);
    world->AddBody(rightArm);
    world->AddBody(leftLeg);
    world->AddBody(rightLeg);
    //
    //// Add joints between ragdoll parts (distance constraints with one anchor point)
    //JointConstraint* string = new JointConstraint(bob, head, bob->position);
    //JointConstraint* neck = new JointConstraint(head, torso, head->position + Vector2D(0, 25));
    //JointConstraint* leftShoulder = new JointConstraint(torso, leftArm, torso->position + Vector2D(-28, -45));
    //JointConstraint* rightShoulder = new JointConstraint(torso, rightArm, torso->position + Vector2D(+28, -45));
    //JointConstraint* leftHip = new JointConstraint(torso, leftLeg, torso->position + Vector2D(-20, +50));
    //JointConstraint* rightHip = new JointConstraint(torso, rightLeg, torso->position + Vector2D(+20, +50));
    //world->AddConstraint(string);
    //world->AddConstraint(neck);
    //world->AddConstraint(leftShoulder);
    //world->AddConstraint(rightShoulder);
    //world->AddConstraint(leftHip);
    //world->AddConstraint(rightHip);
    //
    //// Add a floor and walls to contain objects objects
    //Body* floor = new Body(BoxShape(Graphics::Width() - 50, 50), Graphics::Width() / 2.0, Graphics::Height() - 50, 0.0);
    //Body* leftWall = new Body(BoxShape(50, Graphics::Height() - 100), 50, Graphics::Height() / 2.0 - 25, 0.0);
    //Body* rightWall = new Body(BoxShape(50, Graphics::Height() - 100), Graphics::Width() - 50, Graphics::Height() / 2.0 - 25, 0.0);
    //floor->restitution = 0.7;
    //leftWall->restitution = 0.2;
    //rightWall->restitution = 0.2;
    //world->AddBody(floor);
    //world->AddBody(leftWall);
    //world->AddBody(rightWall);

    
    // Add a big static circle in the middle of the screen
    //Body* bigBall = new Body(CircleShape(64), Graphics::Width() / 2.0, Graphics::Height() / 2.0, 0.0);
    //bigBall->SetTexture("./assets/bowlingball.png");
    //world->AddBody(bigBall);
    /*---------------------------------------------------------------------------------------------------------------*/
    // Load texture for the background image
}

/* Input processing.*/
void Application::Input()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT: running = false; break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) running = false;
                if (event.key.keysym.sym == SDLK_d) debug = !debug;
                if (event.key.keysym.sym == SDLK_UP) world->GetBodies()[0]->ApplyImpulseLinear(Vector2D(0.0, -600.0));
                if (event.key.keysym.sym == SDLK_LEFT) world->GetBodies()[0]->ApplyImpulseLinear(Vector2D(-400.0, 0.0));
                if (event.key.keysym.sym == SDLK_RIGHT) world->GetBodies()[0]->ApplyImpulseLinear(Vector2D(+400.0, 0.0));
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    Body* box = new Body(BoxShape(60, 60), x, y, 1.0);
                    box->SetTexture("./assets/angrybirds/rock-box.png");
                    box->angularVelocity = 0.0;
                    box->friction = 0.9;
                    world->AddBody(box);
                }
                if (event.button.button == SDL_BUTTON_RIGHT)
                {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    Body* rock = new Body(CircleShape(30), x, y, 1.0);
                    rock->SetTexture("./assets/angrybirds/rock-round.png");
                    rock->friction = 0.4;
                    world->AddBody(rock);
                }
                break;

            case SDL_MOUSEMOTION:
            int x, y;
            SDL_GetMouseState(&x, &y);
            Vector2D mouse = Vector2D(x, y);
            Vector2D direction = (mouse - bob->position).Normalize();
            Body* bob = world->GetBodies()[0];
            float speed = 5.0;
            bob->position += direction * speed;
                break;
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------*/
/* Update function (called several times per second to update objects) */
/*----------------------------------------------------------------------------------------------------------------------------------*/
float Application::TimeDeductions()
{
    // Wait some time until the reach the target frame time in milliseconds
    static int timePreviousFrame;
    int timeToWait = PhysicEngine::MILLISECS_PER_FRAME - (SDL_GetTicks() - timePreviousFrame);
    if (timeToWait > 0) SDL_Delay(timeToWait);

    // Calculate the deltatime in seconds
    float deltaTime = (SDL_GetTicks() - timePreviousFrame) / 1000.0f;
    if (deltaTime > 0.016) deltaTime = 0.016;

    // Set the time of the current frame to be used in the next one
    timePreviousFrame = SDL_GetTicks();

    return deltaTime;
}
/*----------------------------------------------------------------------------------------------------------------------------------*/
void Application::Update()
{
    Graphics::ClearScreen(0xFF0F0721);

    /* time deduction. */
    float deltaTime = TimeDeductions();

    /* World update. */
    world->Update(deltaTime);

    /* For Debug Information for collision detetection. */
    //world->SetDebug(debug);
}
/*----------------------------------------------------------------------------------------------------------------------------------*/
/* Render function (called several times per second to draw objects) */
/*----------------------------------------------------------------------------------------------------------------------------------*/
void Application::Render()
{
    Graphics::DrawTexture(Graphics::Width() / 2.0, Graphics::Height() / 2.0, Graphics::Width(), Graphics::Height(), 0.0f, bgTexture);

    // Draw all bodies
    for (auto& body : world->GetBodies())
    {
        if (body->shape->GetType() == CIRCLE)
        {
            CircleShape* circleShape = (CircleShape*)body->shape;
            if (!debug && body->texture)
            {
                Graphics::DrawTexture(
                    body->position.x, body->position.y, circleShape->radius * 2, circleShape->radius * 2, body->rotation, body->texture);
            }
            else if (debug)
            {
                Graphics::DrawCircle(body->position.x, body->position.y, circleShape->radius, body->rotation, 0xFF0000FF);
            }
        }
        if (body->shape->GetType() == BOX)
        {
            BoxShape* boxShape = (BoxShape*)body->shape;
            if (!debug && body->texture)
            {
                Graphics::DrawTexture(body->position.x, body->position.y, boxShape->width, boxShape->height, body->rotation, body->texture);
            }
            else if (debug)
            {
                Graphics::DrawPolygon(body->position.x, body->position.y, boxShape->worldVertices, 0xFF0000FF);
            }
        }
        if (body->shape->GetType() == POLYGON)
        {
            PolygonShape* polygonShape = (PolygonShape*)body->shape;
            if (!debug && body->texture)
            {
                Graphics::DrawTexture(
                    body->position.x, body->position.y, polygonShape->width, polygonShape->height, body->rotation, body->texture);
            }
            else if (debug)
            {
                Graphics::DrawPolygon(body->position.x, body->position.y, polygonShape->worldVertices, 0xFF0000FF);
            }
        }
    }

    Graphics::RenderFrame();
}
/*----------------------------------------------------------------------------------------------------------------------------------*/
void Application::RenderObjects()
{
    //
}
/*----------------------------------------------------------------------------------------------------------------------------------*/
/* Destroy function to delete objects and close the window*/
/*----------------------------------------------------------------------------------------------------------------------------------*/
void Application::Destroy()
{
    delete world;
    Graphics::CloseWindow();
}
/*----------------------------------------------------------------------------------------------------------------------------------*/
