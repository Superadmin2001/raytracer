#ifndef WORLD_H
#define WORLD_H

#include <float.h>

#include "light.h"
#include "intersection.h"

typedef struct world
{
	light  *lights;
	uint32 lightCount;
	shape  **shapes;
	uint32 shapeCount;
} world;

typedef struct comps
{
	float t;
	vec4f point;
	shape *object;
	vec4f eyev;
	vec4f normalv;
	vec4f reflectv;
	uint8 inside;
	vec4f overPoint;
	vec4f underPoint;
	float n1, n2;
} comps;

world worldCreateDefult();
world worldCreate(light *lights, uint32 lightCount, shape **shapes, uint32 shapeCount);

uint8 worldAddSphere(world *w, shape *s);

intersections worldIntersect(world w, ray r);

comps prepareComputations(intersection i, ray r, intersections *is);

uint8 isShadowed(world w, vec4f point);

vec3f worldShade(world *w, comps *c, int32 remaining);
vec3f worldColorAt(world w, ray r, int32 remaining);

void worldDestroy(world *w);

vec4f normalAt(shape *shape, vec4f *worldPoint);

#endif
