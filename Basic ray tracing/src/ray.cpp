#include "ray.h"

ray createRay(vec4f o, vec4f d)
{
	ray result;
	result.origin = o;
	result.direction = d;

	return result;
}

ray rayTransform(ray r, mat4 *m)
{
	r.direction = mat4MulByVec4f(m, &r.direction);
	r.origin = mat4MulByVec4f(m, &r.origin);
	return r;
}

vec4f positionByParam(ray r, float t)
{
	return vec4fAdd(vec4fMulByVal(r.direction, t), r.origin);
}