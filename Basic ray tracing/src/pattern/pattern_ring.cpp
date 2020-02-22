#include "pattern_ring.h"

vec3f pattern_ring_patternAt(pattern_ring *pr, vec4f *point)
{
	if ((int32)floorf(sqrtf(point->x*point->x + point->z*point->z)) % 2 == 0)
		return pr->base.a;

	return pr->base.b;
}

typedef struct pattern_ring_vtable
{
	vec3f(*patternAt)(pattern_ring *pr, vec4f *point);
} pattern_ring_vtable;

static pattern_ring_vtable the_pattern_ring_vtable = {
	pattern_ring_patternAt
};

void pattern_ring_construct_default(pattern_ring *pr)
{
	pattern_construct_default((pattern*)pr);
	pr->base.vptr = (pattern_vtable*)&the_pattern_ring_vtable;
}

void pattern_ring_construct(pattern_ring *pr, vec3f a, vec3f b, mat4 transform)
{
	pattern_construct((pattern*)pr, a, b, transform);
	pr->base.vptr = (pattern_vtable*)&the_pattern_ring_vtable;
}