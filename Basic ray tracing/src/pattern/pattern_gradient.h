#ifndef PATTERN_GRADIENT_H
#define PATTERN_GRADIENT_H

#include "pattern.h"

typedef struct pattern_gradient
{
	pattern base;
} pattern_gradient;

void pattern_gradient_construct_default(pattern_gradient *pg);
void pattern_gradient_construct(pattern_gradient *pg, vec3f a, vec3f b, mat4 transform);

vec3f pattern_gradient_patternAt(pattern_gradient *pg, vec4f *point);

#endif
