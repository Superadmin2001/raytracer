#ifndef PATTERN_H
#define PATTERN_H

#include "..\math\matrices.h"

typedef struct pattern pattern;
typedef struct pattern_vtable
{
	vec3f(*patternAt)(pattern *pn, vec4f *point);
} pattern_vtable;

typedef struct shape shape;

typedef struct pattern
{
	pattern_vtable *vptr;
	vec3f a, b;
	mat4 transform;
} pattern;

void pattern_construct_default(pattern *pn);
void pattern_construct(pattern *pn, vec3f a, vec3f b, mat4 transform);

pattern stripePattern(vec3f a, vec3f b);

vec3f patternAtShape(pattern *pn, shape *object, vec4f *worldPoint);

vec3f stripeAt(pattern *pn, vec4f *point);
vec3f stripeAtObject(shape *object, pattern *pn, vec4f *worldPoint);

void patternSetTransform(pattern *pn, mat4 transform);

#endif