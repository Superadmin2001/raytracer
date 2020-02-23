#include "cylinder.h"
#include "../intersection.h"
#include "../defines.h"

typedef struct cylinder_vtable
{
	int(*intersect)(cylinder *c, intersections *is, ray r);
	vec4f(*normalAt)(cylinder *c, vec4f *worldPoint);
} cylinder_vtable;

static cylinder_vtable the_cylinder_vtable = {
	cylinder_intersect,
	cylinder_normalAt
};

void cylinder_construct_default(cylinder *c)
{
	shape_construct(&c->base, mat4Identity(), createMaterial(createVec3f(0.8f, 1.f, 0.6f), 0.1, 0.7, 0.2, 200.f, 0.f, 0.0f, 1.0f));
	c->base.vptr = (shape_vtable*)&the_cylinder_vtable;
	c->min = FLT_MAX;
	c->max = FLT_MAX;
	c->closed = 0;
}

void cylinder_construct(cylinder *c, float min, float max, uint8 closed, mat4 transform, material material, vec4f position)
{
	shape_construct(&c->base, transform, material);
	c->base.vptr = (shape_vtable*)&the_cylinder_vtable;
	c->min = min;
	c->max = max;
	c->closed = closed;
}

void cylinderSetTransform(cylinder *c, mat4 m)
{
	shape_setTransform((shape*)c, m);
}

void cylinderSetMaterial(cylinder *c, material m)
{
	shape_setMaterial((shape*)c, m);
}

void cylinderSetPattern(cylinder *c, pattern *pn)
{
	pattern p = *pn;
	materialSetPattern(&c->base.material, p);
}

internal uint8 checkCap(ray r, float t)
{
	float x = r.origin.x + t*r.direction.x;
	float z = r.origin.z + t*r.direction.z;

	if((x*x + z*z) <= 1+EPSILON || (x*x + z*z) <= 1-EPSILON)
		return true;

	return false;
}

internal void intersectCaps(cylinder *c, intersections *is, ray r)
{
	if (c->closed == false || fabsf(r.direction.y) <= EPSILON)
		return;

	float t = (c->min - r.origin.y) / r.direction.y;
	if (checkCap(r, t))
		addIntersection(is, createIntersection(t, (shape*)c));

	t = (c->max - r.origin.y) / r.direction.y;
	if (checkCap(r, t))
		addIntersection(is, createIntersection(t, (shape*)c));
}

int cylinder_intersect(cylinder *c, intersections *is, ray r)
{
	intersectCaps(c, is, r);

	float a = r.direction.x * r.direction.x + r.direction.z * r.direction.z;

	if (a <= EPSILON)
		return 0;

	float b = (2 * r.origin.x * r.direction.x) + (2 * r.origin.z * r.direction.z);
	float cc = r.origin.x*r.origin.x + r.origin.z*r.origin.z - 1;

	float d = b*b - 4 * a*cc;

	if (d < 0)
		return 0;

	float t0 = (-b - sqrtf(d)) / (2 * a);
	float t1 = (-b + sqrtf(d)) / (2 * a);

	if (t0 > t1)
		swap2f(t0, t1);

	float y0 = r.origin.y + t0*r.direction.y;
	if (c->min < y0 && y0 < c->max)
		addIntersection(is, createIntersection(t0, (shape*)c));
	
	float y1 = r.origin.y + t1*r.direction.y;
	if (c->min < y1 && y1 < c->max)
		addIntersection(is, createIntersection(t1, (shape*)c));

	return 1;
}

vec4f cylinder_normalAt(cylinder *c, vec4f *point)
{
	float distance = point->x*point->x + point->z*point->z;

	if (distance < 1 && point->y >= c->max - EPSILON)
		return vec4fVector(0, 1, 0);
	else if (distance < 1 && point->y <= c->min + EPSILON)
		return vec4fVector(0, -1, 0);

	return vec4fVector(point->x, 0, point->z);
}