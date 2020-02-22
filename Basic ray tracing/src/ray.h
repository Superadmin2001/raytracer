#ifndef RAY_H
#define RAY_H

#include "math\matrices.h"

typedef struct ray
{
	vec4f origin;
	vec4f direction;
} ray;

ray createRay(vec4f o, vec4f d);
ray rayTransform(ray r, mat4 *m);

vec4f positionByParam(ray r, float t);

#endif
