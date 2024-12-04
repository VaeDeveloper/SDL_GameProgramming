
#include "Display.h"
#include "Vector.h"
#include "Mesh.h"
#include "Triangle.h"

triangle_t TrianglesToRender[N_MESH_FACES];

vec3_t CameraPosition = { .x = 0, .y = 0, .z = -5 };
vec3_t CubeRotation = { .x = 0, .y = 0, .z = 0 };
float FovFactor = 640.0f;
int PreviousFrameTime = 0;
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
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), PreviousFrameTime + FRAME_TARGET_TIME));
	PreviousFrameTime = SDL_GetTicks();

	CubeRotation.x += 0.01;
	CubeRotation.y += 0.01;
	CubeRotation.z += 0.01;


	for (int i = 0; i < N_MESH_FACES; i++)
	{
		face_t meshFace = MeshFaces[i];

		vec3_t faceVerticles[3];
		faceVerticles[0] = MeshVertices[meshFace.a - 1];
		faceVerticles[1] = MeshVertices[meshFace.b - 1];
		faceVerticles[2] = MeshVertices[meshFace.c - 1];
	
		triangle_t projectedTriangle;

		for (int j = 0; j < 3; j++)
		{
			vec3_t transformedVertex = faceVerticles[j];

			transformedVertex = Vec3RotateX(transformedVertex, CubeRotation.x);
			transformedVertex = Vec3RotateY(transformedVertex, CubeRotation.y);
			transformedVertex = Vec3RotateZ(transformedVertex, CubeRotation.z);

			transformedVertex.z -= CameraPosition.z;

			vec2_t projectedPoint = Project(transformedVertex);

			projectedPoint.x += (WindowWidth / 2);
			projectedPoint.y += (WindowHeight / 2);

			projectedTriangle.points[j] = projectedPoint;
		}

		TrianglesToRender[i] = projectedTriangle;
	}
}

void Render(void)
{
	DrawDebugGrid();

	
	for (int i = 0; i < N_MESH_FACES; i++)
	{
		triangle_t triangle = TrianglesToRender[i];
		DrawRect(triangle.points[0].x,triangle.points[0].y,3,3,0xFFFFFF00);
		DrawRect(triangle.points[1].x,triangle.points[1].y,3,3,0xFFFFFF00);
		DrawRect(triangle.points[2].x,triangle.points[2].y,3,3,0xFFFFFF00);

		DrawTriangle
		(
			triangle.points[0].x,
			triangle.points[0].y,
			triangle.points[1].x,
			triangle.points[1].y,
			triangle.points[2].x,
			triangle.points[2].y,
			0xFF00FF00
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

