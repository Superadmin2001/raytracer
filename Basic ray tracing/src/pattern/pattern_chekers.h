#ifndef PATTERN_CHEKERS_H
#define PATTERN_CHEKERS_H

#include "pattern.h"

typedef struct pattern_checkers
{
	pattern base;
} pattern_checkers;

void pattern_checkers_construct_default(pattern_checkers *pc);
void pattern_checkers_construct(pattern_checkers *pc, vec3f a, vec3f b, mat4 transform);

vec3f pattern_checkers_patternAt(pattern_checkers *pc, vec4f *point);

#endif