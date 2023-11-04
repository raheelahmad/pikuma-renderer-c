#pragma once

#include "triangle.h"
#import "vector.h"
// --- Mesh using dynamic vertices and faces.

/// Collection of vertices and faces.
typedef struct {
  vec3_t *vertices; // dynamic array of vertices
  face_t *faces; // dynamic array of faces

  vec3_t rotation; // a rotation for the mesh.
} mesh_t;

// Global mesh
extern mesh_t mesh;

// Load the hard-coded cube data
void load_cube_mesh_data(void);

void load_obj_mesh_data(char *filename);
