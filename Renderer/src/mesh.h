#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include "triangle.h"


#define         N_CUBE_VERTICLES 8
#define         N_CUBE_FACES (6 * 2)

extern          vec3_t mesh_vertices[N_CUBE_VERTICLES];
extern          face_t mesh_faces[N_CUBE_FACES];

/** define a struct for dynamic size meshes, with array of vertices and faces */
typedef struct {
    vec3_t*     vertices;       // dynamic array of vertices
    face_t*     faces;          // dynamic array of faces 
    vec3_t      rotation;       // rotation with x, y, z value
    vec3_t      scale;          // scale 
    vec3_t      translation;    // translation mesh;
}               mesh_t;

/** mesh extern  */
extern mesh_t   mesh;

void            load_cube_mesh_data(void);
void            load_obj_file_data(char* file_name);
#endif