#include "pattern_gradient.h"

vec3f pattern_gradient_patternAt(pattern_gradient *pg, vec4f *point)
{
	vec3f distance = vec3fSub(pg->base.b, pg->base.a);
	float fraction = point->x - floorf(point->x);

	return vec3fAdd(pg->base.a, vec3fMulByVal(distance, fraction));
}

typedef struct pattern_gradient_vtable
{
	vec3f(*patternAt)(pattern_gradient *pg, vec4f *point);
} pattern_gradient_vtable;

static pattern_gradient_vtable the_pattern_gradient_vtable = {
	pattern_gradient_patternAt
};

void pattern_gradient_construct_default(pattern_gradient *pg)
{
	pattern_construct_default((pattern*)pg);
	pg->base.vptr = (pattern_vtable*)&the_pattern_gradient_vtable;
}

void pattern_gradient_construct(pattern_gradient *pg, vec3f a, vec3f b, mat4 transform)
{
	pattern_construct((pattern*)pg, a, b, transform);
	pg->base.vptr = (pattern_vtable*)&the_pattern_gradient_vtable;
}