#include "../include/my_math.h"
#include <math.h>

void mat4_rotate_x(float *m, float angle) {
  float c = cosf(angle);
  float s = sinf(angle);

  m[0] = 1.0f;
  m[4] = 0.0f;
  m[8] = 0.0f;
  m[12] = 0.0f;

  m[1] = 0.0f;
  m[5] = c;
  m[9] = -s;
  m[13] = 0.0f;

  m[2] = 0.0f;
  m[6] = s;
  m[10] = c;
  m[14] = 0.0f;

  m[3] = 0.0f;
  m[7] = 0.0f;
  m[11] = 0.0f;
  m[15] = 1.0f;
}

void mat4_rotate_y(float *m, float angle) {
  float c = cosf(angle);
  float s = sinf(angle);

  m[0] = c;
  m[4] = 0.0f;
  m[8] = s;
  m[12] = 0.0f;

  m[1] = 0.0f;
  m[5] = 1;
  m[9] = 0.0f;
  m[13] = 0.0f;

  m[2] = -s;
  m[6] = 0.0f;
  m[10] = c;
  m[14] = 0.0f;

  m[3] = 0.0f;
  m[7] = 0.0f;
  m[11] = 0.0f;
  m[15] = 1.0f;
}

void mat4_rotate_z(float *m, float angle) {
  float c = cosf(angle);
  float s = sinf(angle);

  m[0] = c;
  m[4] = -s;
  m[8] = 0.0f;
  m[12] = 0.0f;

  m[1] = s;
  m[5] = c;
  m[9] = 0.0f;
  m[13] = 0.0f;

  m[2] = 0.0f;
  m[6] = 0.0f;
  m[10] = 1.0f;
  m[14] = 0.0f;

  m[3] = 0.0f;
  m[7] = 0.0f;
  m[11] = 0.0f;
  m[15] = 1.0f;
}
