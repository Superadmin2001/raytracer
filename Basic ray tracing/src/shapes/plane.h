#ifndef PLANE_H
#define PLANE_H

#include <float.h>

#include "shape.h"

typedef struct plane
{
	shape base;
} plane;

void plane_construct_default(plane *p);
void plane_construct(plane *p, mat4 transform, material material);

void planeSetTransform(plane *p, mat4 transform);
void planeSetPattern(plane *p, pattern *pn);

int plane_intersect(plane *p, intersections *is, ray r);
vec4f plane_normalAt(plane *p, vec4f *point);

#endif
