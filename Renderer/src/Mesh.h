#ifndef MESH_H
#define MESH_H

#include "Vector.h"
#include "Triangle.h"

#define N_CUBE_VERTICLES 8
extern vec3_t mesh_vertices[N_CUBE_VERTICLES];

#define N_CUBE_FACES (6 * 2)
extern face_t mesh_faces[N_CUBE_FACES];

/**
 * Define a struct for dynamic size meshes, with array of vertices and faces  
 */
typedef struct 
{
    vec3_t* vertices;   // dynamic array of vertices
    face_t* faces;      // dynamic array of faces 
    vec3_t rotation;    // rotation with x, y, z value
} mesh_t;


extern mesh_t mesh;

void load_cube_mesh_data(void);
void load_obj_file_data(char* file_name);
#endif