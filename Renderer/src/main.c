
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "array.h"
#include "triangle.h"
#include "utils.h"
#include "matrix.h"
#include "light.h"

#define LOAD_F22 0
#define LOAD_CUBE_MESH 0

/** method debug enm */
enum debug_method{
    DEBUG_NONE,
    DEBUG_GRID
} 	debug_method;

/** culling method enm */
enum cull_method{
    CULL_NONE,
    CULL_BACKFACE
} 	cull_method;

/** render method enm */
enum render_method{
    RENDER_WIRE,
    RENDER_WIRE_VERTEX,
    RENDER_FILL_TRIANGLE,
    RENDER_FILL_TRIANGLE_WIRE
} 	render_method;


mat4_t      projection_matrix;
bool 		is_running 				= false;
bool        b_fullscreen            = false;
int 		previous_frame_time 	= 0;
float       rot_x_offset 			= 0.03f;
float       rot_y_offset 			= 0.03f;
float       rot_z_offset 			= 0.03f;
vec3_t 		camera_position 		= { .x = 0, .y = 0, .z = 0 };
const float rotation_offset_input   = 0.01f;

triangle_t* _triangles_to_render 	= NULL;

/** malloc color buffer hip */
void init_color_buffer(void){
    _color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);
    printf("Sizeof color buffer: %ld \n", sizeof(_color_buffer));
}

/** init color texture buffer  */
void init_color_buffer_texture(void){
    _color_buffer_texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_ARGB8888,
    SDL_TEXTUREACCESS_STREAMING, window_width, window_height);
    printf("Sizeof color buffer texture: %ld \n", sizeof(_color_buffer_texture));
}

void set_fullscreen(SDL_Window* window){
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
}

/** */
void setup_app(void){
	/**                     Setup buffers for texture and color, load mesh data's */
	render_method 		    = RENDER_WIRE;
	cull_method 		    = CULL_BACKFACE;
	debug_method 		    = DEBUG_GRID;

	set_fullscreen(_window);
    init_color_buffer();
    init_color_buffer_texture();

    float fov = 93.0;
    float aspect = (float)window_height / (float)window_width;
    float znear = 0.1;
    float zfar = 100.0;
    projection_matrix = mat4_make_perspective(fov, aspect, znear, zfar);

//    load_cube_mesh_data();

    /* loading mesh obj */
    load_obj_file_data("./assets/crab.obj");
}

/**  input proccesing with sdl events */
void process_input(void){
	SDL_Event event;
	SDL_PollEvent(&event);

	switch(event.type){
		case SDL_QUIT:{
			is_running = false;
			break;
		}

		case SDL_KEYDOWN:{
			if (event.key.keysym.sym == SDLK_ESCAPE){
				is_running = false;
			}
            if (event.key.keysym.sym == SDLK_5){
				b_fullscreen = !b_fullscreen;
			}
			if (event.key.keysym.sym == SDLK_1){
				render_method = RENDER_WIRE_VERTEX;
			}
			if (event.key.keysym.sym == SDLK_2){
				render_method = RENDER_WIRE;
			}
			if (event.key.keysym.sym == SDLK_3){
				render_method = RENDER_FILL_TRIANGLE;
			}
			if (event.key.keysym.sym == SDLK_4){
				render_method = RENDER_FILL_TRIANGLE_WIRE;
			}
			if (event.key.keysym.sym == SDLK_c){
				cull_method = CULL_BACKFACE;
			}
			if (event.key.keysym.sym == SDLK_v){
				cull_method = CULL_NONE;
			}
			if (event.key.keysym.sym == SDLK_d){
				debug_method = ~debug_method;
			}
			if (event.key.keysym.sym == SDLK_n){
				debug_method = DEBUG_NONE;
			}

            // change mesh rotation inputs   
            if (event.key.keysym.sym == SDLK_w){
				rot_x_offset += rotation_offset_input;
			}

            if (event.key.keysym.sym == SDLK_q){
                rot_x_offset -= rotation_offset_input;
            } 

            if (event.key.keysym.sym == SDLK_x){
                rot_y_offset += rotation_offset_input;
            }
            if (event.key.keysym.sym == SDLK_z){
                rot_y_offset -= rotation_offset_input;
            }
            if (event.key.keysym.sym == SDLK_s){
                rot_z_offset += rotation_offset_input;
            }
		}
	}
}

/** update scale rotation and translation mesh */
void mesh_update(void){
    mesh.rotation.x        += rotation_offset_input;
    mesh.rotation.y        += rotation_offset_input + 0.02;
    mesh.rotation.z        += rotation_offset_input + 0.03;
     //mesh.translation.x     -= 0.01;
    mesh.translation.z     = 5;
}

void update(void){
    // Wait some time until the reach the target frame time in milliseconds
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);

    // Only delay execution if we are running too fast
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
        SDL_Delay(time_to_wait);
    }

    previous_frame_time = SDL_GetTicks();

    // Initialize the array of triangles to render
    _triangles_to_render = NULL;

    // Change the mesh scale, rotation, and translation values per animation frame
    mesh_update();

    // Create scale, rotation, and translation matrices that will be used to multiply the mesh vertices
    mat4_t scale_matrix = mat4_make_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);
    mat4_t translation_matrix = mat4_make_translation(mesh.translation.x, mesh.translation.y, mesh.translation.z);
    mat4_t rotation_matrix_x = mat4_make_rotation_x(mesh.rotation.x);
    mat4_t rotation_matrix_y = mat4_make_rotation_y(mesh.rotation.y);
    mat4_t rotation_matrix_z = mat4_make_rotation_z(mesh.rotation.z);

    // Loop all triangle faces of our mesh
    int num_faces = array_length(mesh.faces);

    for (int i = 0; i < num_faces; i++) {
        face_t mesh_face = mesh.faces[i];

        vec3_t face_vertices[3];
        face_vertices[0] = mesh.vertices[mesh_face.a - 1];
        face_vertices[1] = mesh.vertices[mesh_face.b - 1];
        face_vertices[2] = mesh.vertices[mesh_face.c - 1];

        vec4_t transformed_vertices[3];

        // Loop all three vertices of this current face and apply transformations
        for (int j = 0; j < 3; j++) {
            vec4_t transformed_vertex = vec4_from_vec3(face_vertices[j]);

            // Create a World Matrix combining scale, rotation, and translation matrices
            mat4_t world_matrix = mat4_identity();

            // Order matters: First scale, then rotate, then translate. [T]*[R]*[S]*v
            world_matrix = mat4_multiply_mat4(scale_matrix, world_matrix);
            world_matrix = mat4_multiply_mat4(rotation_matrix_z, world_matrix);
            world_matrix = mat4_multiply_mat4(rotation_matrix_y, world_matrix);
            world_matrix = mat4_multiply_mat4(rotation_matrix_x, world_matrix);
            world_matrix = mat4_multiply_mat4(translation_matrix, world_matrix);

            // Multiply the world matrix by the original vector
            transformed_vertex = mat4_multiply_vec4(world_matrix, transformed_vertex);

            // Save transformed vertex in the array of transformed vertices
            transformed_vertices[j] = transformed_vertex;
        }

        // Get individual vectors from A, B, and C vertices to compute normal
        vec3_t vector_a = vec3_from_vec4(transformed_vertices[0]); /*   A   */
        vec3_t vector_b = vec3_from_vec4(transformed_vertices[1]); /*  / \  */
        vec3_t vector_c = vec3_from_vec4(transformed_vertices[2]); /* C---B */

        // Get the vector subtraction of B-A and C-A
        vec3_t vector_ab = vec3_subtract(vector_b, vector_a);
        vec3_t vector_ac = vec3_subtract(vector_c, vector_a);
        vec3_normalize(&vector_ab);
        vec3_normalize(&vector_ac);

        // Compute the face normal (using cross product to find perpendicular)
        vec3_t normal = vec3_cross_product(vector_ab, vector_ac);
        vec3_normalize(&normal);

        // Find the vector between vertex A in the triangle and the camera origin
        vec3_t camera_ray = vec3_subtract(camera_position, vector_a);

        // Calculate how aligned the camera ray is with the face normal (using dot product)
        float dot_normal_camera = vec3_dot_product(normal, camera_ray);

        // Backface culling test to see if the current face should be projected
        if (cull_method == CULL_BACKFACE) {
            // Backface culling, bypassing triangles that are looking away from the camera
            if (dot_normal_camera < 0) {
                continue;
            }
        }

        vec4_t projected_points[3];

        // Loop all three vertices to perform projection
        for (int j = 0; j < 3; j++) {
            // Project the current vertex
            projected_points[j] = mat4_multiply_vec4_project(projection_matrix, transformed_vertices[j]);

            // Scale into the view
            projected_points[j].x *= (window_width / 2.0);
            projected_points[j].y *= (window_height / 2.0);

            // Translate the projected points to the middle of the screen
            projected_points[j].x += (window_width / 2.0);
            projected_points[j].y += (window_height / 2.0);
        }

        // Calculate the average depth for each face based on the vertices after transformation
        float avg_depth = (transformed_vertices[0].z + transformed_vertices[1].z + transformed_vertices[2].z) / 3.0;

        // Calculate the shade intensity based on how aliged is the face normal and the opposite of the light direction
        float light_intensity_factor = -vec3_dot_product(normal, light.direction);

        // Calculate the triangle color based on the light angle
        uint32_t triangle_color = light_apply_intensity(mesh_face.color, light_intensity_factor);

        triangle_t projected_triangle = {
            .points = {
                { projected_points[0].x, projected_points[0].y },
                { projected_points[1].x, projected_points[1].y },
                { projected_points[2].x, projected_points[2].y },
            },
            .color = triangle_color,
            .avg_depth = avg_depth
        };

        // Save the projected triangle in the array of triangles to render
        array_push(_triangles_to_render, projected_triangle);
    }

    // Sort the triangles to render by their avg_depth
    int num_triangles = array_length(_triangles_to_render);

    for (int i = 0; i < num_triangles; i++) {
        for (int j = i; j < num_triangles; j++) {
            if (_triangles_to_render[i].avg_depth < _triangles_to_render[j].avg_depth) {
                // Swap the triangles positions in the array
                triangle_t temp = _triangles_to_render[i];
                _triangles_to_render[i] = _triangles_to_render[j];
                _triangles_to_render[j] = temp;
            }
        }
    }

}

// Render function to draw objects on the display
void render(void) {
    SDL_RenderClear(_renderer);

	if (debug_method == DEBUG_GRID) draw_debug_grid();
    	
    // Loop all projected triangles and render them
    int num_triangles = array_length(_triangles_to_render);
    for (int i = 0; i < num_triangles; i++) {
        triangle_t triangle = _triangles_to_render[i];

        // Draw filled triangle
        if (render_method == RENDER_FILL_TRIANGLE || render_method == RENDER_FILL_TRIANGLE_WIRE) {
            draw_filled_triangle(
                triangle.points[0].x, triangle.points[0].y, // vertex A
                triangle.points[1].x, triangle.points[1].y, // vertex B
                triangle.points[2].x, triangle.points[2].y, // vertex C
                triangle.color
            );
        }

        // Draw triangle wireframe
        if (render_method == RENDER_WIRE || render_method == RENDER_WIRE_VERTEX || render_method == RENDER_FILL_TRIANGLE_WIRE) {
            draw_triangle(
                triangle.points[0].x, triangle.points[0].y, // vertex A
                triangle.points[1].x, triangle.points[1].y, // vertex B
                triangle.points[2].x, triangle.points[2].y, // vertex C
                0xFFFFFFFF
            );
        }

        // Draw triangle vertex points
        if (render_method == RENDER_WIRE_VERTEX) {
            draw_rect(triangle.points[0].x - 3, triangle.points[0].y - 3, 6, 6, 0xFFFF0000); // vertex A
            draw_rect(triangle.points[1].x - 3, triangle.points[1].y - 3, 6, 6, 0xFFFF0000); // vertex B
            draw_rect(triangle.points[2].x - 3, triangle.points[2].y - 3, 6, 6, 0xFFFF0000); // vertex C
        }
    }

    // Clear the array of triangles to render every frame loop
    array_free(_triangles_to_render);
    render_color_buffer();
    clear_color_buffer(0xFF000000);

    SDL_RenderPresent(_renderer);
}

void free_resources(void){
	free(_color_buffer);
	array_free(mesh.faces);
	array_free(mesh.vertices);
}

int main(int argc, char *argv[]){
    (void)argc;
    (void)argv;

	is_running = initialize_window();
	setup_app();

	while(is_running){
		process_input();
		update();
		render();
	}

	destroy_window();
	free_resources();
	return 0;
}

