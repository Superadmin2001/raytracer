#ifndef SHAPE_H
#define SHAPE_H

#include "material.h"
#include "ray.h"

typedef struct intersections intersections;

typedef struct shape shape;

typedef struct shape_vtable
{
	int (*intersect)(shape *s, intersections *is, ray r);
	vec4f(*normalAt)(shape *s, vec4f *point);
} shape_vtable;

struct shape
{
	shape_vtable *vptr;
	mat4 transform;
	material material;
};

void shape_construct(shape *s, mat4 transform, material material);

void shape_setMaterial(shape *s, material material);
void shape_setTransform(shape *s, mat4 transform);

int shape_intersect(shape *s, intersections *is, ray r);

vec4f shape_normalAt(shape *s, vec4f *point);

#endif
