#pragma once
typedef struct {
  float x;
  float y;
} vec2_t;

typedef struct {
  float x;
  float y;
  float z;
} vec3_t;

vec3_t rotate_x(vec3_t p, float angle);
vec3_t rotate_y(vec3_t p, float angle);
vec3_t rotate_z(vec3_t p, float angle);

vec3_t rotate(vec3_t point, vec3_t rotation);
