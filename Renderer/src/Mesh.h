#ifndef MESH_H
#define MESH_H

#include "Vector.h"
#include "Triangle.h"

#define N_MESH_VERTICLES 8
extern vec3_t MeshVertices[N_MESH_VERTICLES];

#define N_MESH_FACES (6 * 2)
extern face_t MeshFaces[N_MESH_FACES];



#endif