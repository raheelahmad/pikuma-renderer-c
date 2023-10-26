#include "vector.h"
#include <math.h>

vec3_t rotate_x(vec3_t p, float angle) {
  vec3_t rotated_point = {.x = p.x,
                          .y = p.y * sin(angle) + p.z * cos(angle),
                          .z = p.y * cos(angle) - p.z * sin(angle)};
  return rotated_point;
}

vec3_t rotate_y(vec3_t p, float angle) {
  vec3_t rotated_point = {.x = p.x * cos(angle) - p.z * sin(angle),
                          .y = p.y,
                          .z = p.x * sin(angle) + p.z * cos(angle)};
  return rotated_point;
}

vec3_t rotate_z(vec3_t p, float angle) {
  vec3_t rotated_point = {.x = p.x * cos(angle) - p.y * sin(angle),
                          .y = p.x * sin(angle) + p.y * cos(angle),
                          .z = p.z};
  return rotated_point;
}
