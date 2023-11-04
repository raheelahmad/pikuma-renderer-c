#pragma once

#include "triangle.h"
#import "vector.h"

// --- Older "cube"

#define N_CUBE_VERTICES 8
#define N_CUBE_FACES (6 * 2) // 6 cube faces, 2 triangles per face

extern vec3_t cube_vertices[N_CUBE_VERTICES];
extern face_t cube_faces[N_CUBE_FACES];

// --- Mesh using dynamic vertices and faces.

/// Collection of vertices and faces.
typedef struct {
  vec3_t *vertices; // dynamic array of vertices
  face_t *faces; // dynamic array of faces

  vec3_t rotation; // a rotation for the mesh.
} mesh_t;

// Global mesh
extern mesh_t mesh;

void load_cube_mesh_data(void);
