#ifndef CUBE_H
#define CUBE_H

#include "../shape.h"

typedef struct intersections intersections;

typedef struct cube
{
	shape base;
	vec4f position;
} cube;

void cube_construct_default(cube *s);
void cube_construct(cube *c, mat4 transform, material material, vec4f position);

void cubeSetTransform(cube *c, mat4 m);
void cubeSetMaterial(cube *c, material m);
void cubeSetPattern(cube *c, pattern *pn);

int cube_intersect(cube *c, intersections *is, ray r);

vec4f cube_normalAt(cube *c, vec4f *point);

#endif