
#include "Display.h"
#include "Vector.h"

#define NPOINTS (9 * 9 * 9)
vec3_t CubePoints[NPOINTS];
vec2_t ProjectedPoints[NPOINTS];
vec3_t CameraPosition = { .x = 0, .y = 0, .z = -5 };
vec3_t CubeRotation = { .x = 0, .y = 0, .z = 0 };
float FovFactor = 640.0f;
bool IsRunning = false;

void Setup(void)
{
	ColorBuffer = (uint32_t*)malloc(sizeof(uint32_t) * WindowWidth * WindowHeight);

	ColorBufferTexture = SDL_CreateTexture
	(
		Renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		WindowWidth,
		WindowHeight
	);

	int PointsCount = 0;

	for (float x = -1.0f; x <= 1.0f; x += 0.25f)
	{
		for (float y = -1.0f; y <= 1.0f; y += 0.25f)
		{
			for (float z = -1.0f; z <= 1.0f; z += 0.25f)
			{
				vec3_t newPoints = { .x = x, .y = y, .z = z };
				CubePoints[PointsCount++] = newPoints;
			}
		}
	}

}

void ProcessInput(void)
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch(event.type)
	{
		case SDL_QUIT:
			IsRunning = false;
			break;

		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				IsRunning = false;
			}

	}
}

vec2_t Project(vec3_t point)
{
	vec2_t projectedPoints = 
	{
		.x = (FovFactor * point.x) / point.z,
		.y = (FovFactor * point.y) / point.z,
	};
	return projectedPoints;
}




void Update(void)
{
	CubeRotation.x += 0.01;
	CubeRotation.y += 0.01;
	CubeRotation.z += 0.01;

	for (int i = 0; i < NPOINTS; i++)
	{
		vec3_t point = CubePoints[i];

		vec3_t transformedPoint = Vec3RotateX(point, CubeRotation.x);
		transformedPoint = Vec3RotateY(transformedPoint, CubeRotation.y);
		transformedPoint = Vec3RotateZ(transformedPoint, CubeRotation.z);

		transformedPoint.z -= CameraPosition.z;

		vec2_t projectedPoints = Project(transformedPoint);

		ProjectedPoints[i] = projectedPoints;
	}
}

void Render(void)
{
	DrawDebugGrid();

	for (int i = 0; i < NPOINTS; i++)
	{
		vec2_t projPoint = ProjectedPoints[i];
		DrawRect
		(
			projPoint.x + (WindowWidth / 2),
			projPoint.y + (WindowHeight / 2),
			4,
			4,
			0xFFFFFF00
		);
	}

	RenderColorBuffer();
	ClearColorBuffer(0xFF000000);

	SDL_RenderPresent(Renderer);

}

int main(void)
{
	IsRunning = InitializeWindow();

	Setup();

	while(IsRunning)
	{
		ProcessInput();
		Update();
		Render();
	}

	DestroyWindow();

	return 0;
}

