
#include "Display.h"
#include "Vector.h"
#include "Mesh.h"
#include "array.h"
#include "Triangle.h"


bool is_running = false;
int previous_frame_time = 0;
float fov_factor = 640.0f;
vec3_t camera_position = { 0, 0, 0 };

triangle_t* _triangles_to_render = NULL;

/**
 * 
 */
void setup(void)
{
	_color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);

	_color_buffer_texture = SDL_CreateTexture
	(
		_renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		window_width,
		window_height
	);

	// load_cube_mesh_data();
	load_obj_file_data("./assets/cube.obj");
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

		vec3_t vector_a = transformed_vertices[0];	/*	 A	 */
		vec3_t vector_b = transformed_vertices[1];	/*	/ \	 */
		vec3_t vector_c = transformed_vertices[2];	/* C---B */

		vec3_t vector_ab = vec3_sub(vector_b, vector_a);
		vec3_t vector_ac = vec3_sub(vector_c, vector_a);
		vec3_normalize(&vector_ab);
		vec3_normalize(&vector_ac);

		vec3_t normal = vec3_cross(vector_ab, vector_ac);
		vec3_normalize(&normal);
		vec3_t camera_ray = vec3_sub(camera_position, vector_a);

		float dot_normal_camera = vec3_dot(normal, camera_ray);

		triangle_t projected_triangle;

		if (dot_normal_camera < 0)
		{
			continue;
		}

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
	draw_debug_grid();

	int num_triangles = array_length(_triangles_to_render);

	for (int i = 0; i < num_triangles; i++)
	{
		triangle_t triangle = _triangles_to_render[i];
		draw_rect(triangle.points[0].x,triangle.points[0].y,3,3,0xFFFFFF00);
		draw_rect(triangle.points[1].x,triangle.points[1].y,3,3,0xFFFFFF00);
		draw_rect(triangle.points[2].x,triangle.points[2].y,3,3,0xFFFFFF00);

		draw_triangle
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

