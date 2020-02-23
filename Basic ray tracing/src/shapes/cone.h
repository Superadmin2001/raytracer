#ifndef CONE_H
#define CONE_H

#include "../shape.h"

typedef struct cone
{
	shape base;
	float min;
	float max;
	uint8 closed;
} cone;

void cone_construct_default(cone *c);
void cone_construct(cone *c, float min, float max, uint8 closed, mat4 transform, material material, vec4f position);

void coneSetTransform(cone *c, mat4 m);
void coneSetMaterial(cone *c, material m);
void coneSetPattern(cone *c, pattern *pn);

int cone_intersect(cone *c, intersections *is, ray r);

vec4f cone_normalAt(cone *c, vec4f *point);

#endif
