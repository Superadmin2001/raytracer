#ifndef PATTERN_RING_H
#define PATTERN_RING_H

#include "pattern.h"

typedef struct pattern_ring
{
	pattern base;
} pattern_ring;

void pattern_ring_construct_default(pattern_ring *pr);
void pattern_ring_construct(pattern_ring *pr, vec3f a, vec3f b, mat4 transform);

vec3f pattern_ring_patternAt(pattern_ring *pr, vec4f *point);

#endif
