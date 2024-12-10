
#include "Display.h"
#include "Vector.h"
#include "Mesh.h"
#include "array.h"
#include "Triangle.h"

#define LOAD_CUBE_MESH 1

enum debug_method
{
    DEBUG_NONE,
    DEBUG_GRID
} debug_method;

enum cull_method
{
    CULL_NONE,
    CULL_BACKFACE
} cull_method;

enum render_method
{
    RENDER_WIRE,
    RENDER_WIRE_VERTEX,
    RENDER_FILL_TRIANGLE,
    RENDER_FILL_TRIANGLE_WIRE
} render_method;


bool is_running = false;
int previous_frame_time = 0;
float fov_factor = 640.0f;
vec3_t camera_position = { 0, 0, 0 };

triangle_t* _triangles_to_render = NULL;

/**
 * Sutup buffers for texture and color, load mesh data's
 */
void setup(void)
{
	render_method = RENDER_WIRE;
	cull_method = CULL_BACKFACE;
	debug_method = DEBUG_GRID;

	_color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);

	_color_buffer_texture = SDL_CreateTexture
	(
		_renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		window_width,
		window_height
	);


#if LOAD_CUBE_MESH
	load_obj_file_data("./assets/cube.obj");
#else
	load_obj_file_data("./assets/f22.obj");
#endif
}

/**
 * 
 */
void process_input(void)
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch(event.type)
	{
		case SDL_QUIT:
			is_running = false;
			break;

		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				is_running = false;
			}
			if (event.key.keysym.sym == SDLK_1)
			{
				render_method = RENDER_WIRE_VERTEX;
			}
			if (event.key.keysym.sym == SDLK_2)
			{
				render_method = RENDER_WIRE;
			}
			if (event.key.keysym.sym == SDLK_3)
			{
				render_method = RENDER_FILL_TRIANGLE;
			}
			if (event.key.keysym.sym == SDLK_4)
			{
				render_method = RENDER_FILL_TRIANGLE_WIRE;
			}
			if (event.key.keysym.sym == SDLK_c)
			{
				cull_method = CULL_BACKFACE;
			}
			if (event.key.keysym.sym == SDLK_v)
			{
				cull_method = CULL_NONE;
			}
			if (event.key.keysym.sym == SDLK_d)
			{
				debug_method = DEBUG_GRID;
			}
			if (event.key.keysym.sym == SDLK_n)
			{
				debug_method = DEBUG_NONE;
			}

	}
}

vec2_t project(vec3_t point)
{
	vec2_t projected_points = 
	{
		.x = (fov_factor * point.x) / point.z,
		.y = (fov_factor * point.y) / point.z,
	};

	return projected_points;
}

void update(void)
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), previous_frame_time + FRAME_TARGET_TIME));
	previous_frame_time = SDL_GetTicks();

	_triangles_to_render = NULL;

	mesh.rotation.x += 0.01;
	mesh.rotation.y += 0.01;
	mesh.rotation.z += 0.01;

	int num_faces = array_length(mesh.faces);

	for (int i = 0; i < num_faces; i++)
	{
		face_t mesh_face = mesh.faces[i];

		vec3_t face_verticles[3];
		face_verticles[0] = mesh.vertices[mesh_face.a - 1];
		face_verticles[1] = mesh.vertices[mesh_face.b - 1];
		face_verticles[2] = mesh.vertices[mesh_face.c - 1];
		
		vec3_t transformed_vertices[3];

		for (int j = 0; j < 3; j++)
		{
			vec3_t transformed_vertex = face_verticles[j];

			transformed_vertex = vec3_rotate_x(transformed_vertex, mesh.rotation.x);
			transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotation.y);
			transformed_vertex = vec3_rotate_z(transformed_vertex, mesh.rotation.z);

			transformed_vertex.z += 5;

			transformed_vertices[j] = transformed_vertex;
		}

		if (cull_method == CULL_BACKFACE)
		{
					/* Check backface culling */
			vec3_t vector_a = transformed_vertices[0];	/*	 A	 */
			vec3_t vector_b = transformed_vertices[1];	/*	/ \	 */
			vec3_t vector_c = transformed_vertices[2];	/* C---B */

			/* Get the vector subtraction of B-A and C-A */
			vec3_t vector_ab = vec3_sub(vector_b, vector_a);
			vec3_t vector_ac = vec3_sub(vector_c, vector_a);
			vec3_normalize(&vector_ab);
			vec3_normalize(&vector_ac);

			/* Cumpute the face normal (using cross product to find perpendicular ) */
			vec3_t normal = vec3_cross(vector_ab, vector_ac);
			vec3_normalize(&normal);
		
			/* Find the vector between a point in the triangle and the camera origin */
			vec3_t camera_ray = vec3_sub(camera_position, vector_a);

			/* Calculate how alligned the camera ray is with the face normal (using dot product) */
			float dot_normal_camera = vec3_dot(normal, camera_ray);


			if (dot_normal_camera < 0) continue;
		}


		triangle_t projected_triangle;

		for (int j = 0; j < 3; j++)
		{
			vec2_t projected_point = project(transformed_vertices[j]);

			projected_point.x += (window_width / 2);
			projected_point.y += (window_height / 2);

			projected_triangle.points[j] = projected_point;
		}

		array_push(_triangles_to_render, projected_triangle);
	}
}

void render(void)
{
	if (debug_method == DEBUG_GRID)
	{
		draw_debug_grid();
	}
	

	int num_triangles = array_length(_triangles_to_render);

	for (int i = 0; i < num_triangles; i++)
	{
		triangle_t triangle = _triangles_to_render[i];
		
		if (render_method == RENDER_FILL_TRIANGLE || render_method == RENDER_FILL_TRIANGLE_WIRE)
		{
			draw_filled_triangle
			(
				triangle.points[0].x, triangle.points[0].y,
				triangle.points[1].x, triangle.points[1].y,
				triangle.points[2].x, triangle.points[2].y,
				0xFF555555
			);
		}

		if (render_method == RENDER_WIRE || render_method == RENDER_WIRE_VERTEX || render_method == RENDER_FILL_TRIANGLE_WIRE)
		{
			draw_triangle
			(
				triangle.points[0].x, triangle.points[0].y,
				triangle.points[1].x, triangle.points[1].y,
				triangle.points[2].x, triangle.points[2].y,
				0xFFFFFFFF
			);
		}

		if (render_method == RENDER_WIRE_VERTEX)
		{
			draw_rect(triangle.points[0].x - 3, triangle.points[0].y - 3, 6, 6, 0xFFFF0000);
			draw_rect(triangle.points[1].x - 3, triangle.points[2].y - 3, 6, 6, 0xFFFF0000);
			draw_rect(triangle.points[2].x - 3, triangle.points[2].y - 3, 6, 6, 0xFFFF0000);

		}

	}
	
	array_free(_triangles_to_render);
	render_color_buffer();
	clear_color_buffer(0xFF000000);

	SDL_RenderPresent(_renderer);

}

void free_resources(void)
{
	free(_color_buffer);
	array_free(mesh.faces);
	array_free(mesh.vertices);
}

int main(void)
{
	is_running = initialize_window();

	setup();

	while(is_running)
	{
		process_input();
		update();
		render();
	}

	destroy_window();
	free_resources();
	return 0;
}

