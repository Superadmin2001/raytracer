#include "pattern.h"

#include <math.h>

#include "../constants.h"
#include "../shape.h"

vec3f stripeAt(pattern *pn, vec4f *point)
{
	if ((int32)floorf(point->x) % 2 == 0)
		return pn->a;

	return pn->b;
}

static pattern_vtable the_pattern_vtable
{
	stripeAt
};

void pattern_construct_default(pattern *pn)
{
	pn->vptr = &the_pattern_vtable;
	pn->a = white_color;
	pn->b = black_color;
	pn->transform = mat4Identity();
}

void pattern_construct(pattern *pn, vec3f a, vec3f b, mat4 transform)
{
	pn->vptr = &the_pattern_vtable;
	pn->a = a;
	pn->b = b;
	pn->transform = transform;
}

vec3f patternAtShape(pattern *pn, shape *object, vec4f *worldPoint)
{
	vec4f localPoint = mat4MulByVec4f(&mat4Inverse(&object->transform), worldPoint);
	vec4f localPattern = mat4MulByVec4f(&mat4Inverse(&pn->transform), &localPoint);

	return pn->vptr->patternAt(pn, &localPattern);
}

pattern stripePattern(vec3f a, vec3f b)
{
	pattern result;
	result.a = a;
	result.b = b;
	result.transform = mat4Identity();

	return result;
}

vec3f stripeAtObject(shape *object, pattern *pn, vec4f *worldPoint)
{
	vec4f localPoint = mat4MulByVec4f(&mat4Inverse(&object->transform), worldPoint);
	vec4f localPattern = mat4MulByVec4f(&mat4Inverse(&pn->transform), &localPoint);

	return stripeAt(pn, &localPattern);
}

void patternSetTransform(pattern *pn, mat4 transform)
{
	pn->transform = transform;
}