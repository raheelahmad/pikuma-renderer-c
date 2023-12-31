#import "mesh.h"
#include "triangle.h"
#include "vector.h"
#include <string.h>
#import "array.h"
#include <_stdio.h>
#include <stdio.h>

// --- Older "cube"

#define N_CUBE_VERTICES 8
#define N_CUBE_FACES (6 * 2) // 6 cube faces, 2 triangles per face

mesh_t mesh = {.vertices = NULL, .faces = NULL, .rotation = {0, 0, 0}};

vec3_t cube_vertices[N_CUBE_VERTICES] = {
    {.x = -1, .y = -1, .z = -1}, // 1
    {.x = -1, .y = 1, .z = -1},  // 2
    {.x = 1, .y = 1, .z = -1},   // 3
    {.x = 1, .y = -1, .z = -1},  // 4
    {.x = 1, .y = 1, .z = 1},    // 5
    {.x = 1, .y = -1, .z = 1},   // 6
    {.x = -1, .y = 1, .z = 1},   // 7
    {.x = -1, .y = -1, .z = 1},  // 8
};

face_t cube_faces[N_CUBE_FACES] = {
    // front
    {.a = 1, .b = 2, .c = 3},
    {.a = 1, .b = 3, .c = 4},

    // right
    {.a = 4, .b = 3, .c = 5},
    {.a = 4, .b = 5, .c = 6},

    {.a = 6, .b = 5, .c = 7},
    {.a = 6, .b = 7, .c = 8},

    {.a = 8, .b = 7, .c = 2},
    {.a = 8, .b = 2, .c = 1},

    {.a = 2, .b = 7, .c = 5},
    {.a = 2, .b = 5, .c = 3},

    {.a = 6, .b = 8, .c = 1},
    {.a = 6, .b = 1, .c = 4},
};

void load_cube_mesh_data(void) {
  for (int i = 0; i < N_CUBE_VERTICES; ++i) {
    array_push(mesh.vertices, cube_vertices[i]);
  }
  for (int i = 0; i < N_CUBE_FACES; ++i) {
    array_push(mesh.faces, cube_faces[i]);
  }
}

void load_obj_mesh_data(char *filename) {
  // Read the contents of the file
  // load the vertices and faces in to mesh.vertices and mesh.faces

  FILE *file;
  file = fopen(filename, "r");

  int numchars = 512; // random length
  char line[numchars];
  while (fgets(line, numchars, file)) {
    // Parse if there is a vertex in this line and store
    if (strncmp(line, "v ", 2) == 0) {
      vec3_t vertex;
      sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
      array_push(mesh.vertices, vertex);
    }

    // Parse if there is a face in this line and store
    if (strncmp(line, "f ", 2) == 0) {
      face_t face;
      int vertex_indices[3];
      int texture_indices[3];
      int normal_indices[3];
      sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
             &vertex_indices[0],
             &texture_indices[0],
             &normal_indices[0],

             &vertex_indices[1],
             &texture_indices[1],
             &normal_indices[1],

             &vertex_indices[2],
             &texture_indices[2],
             &normal_indices[2]
);
      face.a = vertex_indices[0];
      face.b = vertex_indices[1];
      face.c = vertex_indices[2];
      array_push(mesh.faces, face);
    }
  }

  fclose(file);
}
