#ifndef MODEL_H
#define MODEL_H

#include "cglm/types.h"

typedef struct {
  vec3 pos, rot, scale;
} model;

// create default model
model model_new();
// create mat4 of model
void model_matrix(model *dest, mat4 m);

#endif
