#ifndef GROUP_H
#define GROUP_H

#include "../math/matrices.h"
#include "shape.h"

typedef struct group
{
	shape  base;
	uint32 capacity;
	uint32 count;
	shape  **shapes;
} group;

void groupConstructDefault(group *g);

int groupIntersect(group *g, intersections *is, ray r);

vec4f groupNormalAt(group *g, vec4f *worldPoint);

void groupSetTransform(group *g, mat4 m);
void groupSetMaterial(group *g, material m);
void groupSetPattern(group *g, pattern *pn);

uint8 groupAddShape(group *g, shape *s);

void groupPrintShapesAddresses(group *g);

#endif