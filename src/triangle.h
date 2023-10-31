#pragma once

#include "vector.h"
typedef struct {
  // indices
  int a;
  int b;
  int c;
} face_t;

// Stores the projected points on to the screen.
typedef struct {
  vec2_t points[3];
} triangle_t;