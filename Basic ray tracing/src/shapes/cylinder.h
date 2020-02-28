#ifndef CYLINDER_H
#define CYLIDNER_H

#include "shape.h"

typedef struct cylinder
{
	shape base;
	float min;
	float max;
	uint8 closed;
} cylinder;

void cylinder_construct_default(cylinder *c);
void cylinder_construct(cylinder *c, float min, float max, uint8 closed, mat4 transform, material material, vec4f position);

void cylinderSetTransform(cylinder *c, mat4 m);
void cylinderSetMaterial(cylinder *c, material m);
void cylinderSetPattern(cylinder *c, pattern *pn);

int cylinder_intersect(cylinder *c, intersections *is, ray r);

vec4f cylinder_normalAt(cylinder *c, vec4f *point);


#endif
