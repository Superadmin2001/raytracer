#include "pattern_chekers.h"

vec3f pattern_checkers_patternAt(pattern_checkers *pc, vec4f *point)
{
	if ( (int32)(floorf(point->x) + floorf(point->y) + floorf(point->z)) % 2 == 0)
		return pc->base.a;

	return pc->base.b;
}

typedef struct pattern_checkers_vtable
{
	vec3f(*patternAt)(pattern_checkers *pc, vec4f *point);
} pattern_checkers_vtable;

static pattern_checkers_vtable the_pattern_checkers_vtable = {
	pattern_checkers_patternAt
};

void pattern_checkers_construct_default(pattern_checkers *pc)
{
	pattern_construct_default((pattern*)pc);
	pc->base.vptr = (pattern_vtable*)&the_pattern_checkers_vtable;
}

void pattern_checkers_construct(pattern_checkers *pc, vec3f a, vec3f b, mat4 transform)
{
	pattern_construct((pattern*)pc, a, b, transform);
	pc->base.vptr = (pattern_vtable*)&the_pattern_checkers_vtable;
}