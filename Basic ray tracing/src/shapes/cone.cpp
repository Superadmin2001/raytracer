#include "cone.h"
#include "../intersection.h"
#include "../defines.h"

typedef struct cone_vtable
{
	int(*intersect)(cone *c, intersections *is, ray r);
	vec4f(*normalAt)(cone *c, vec4f *worldPoint);
} cylinder_vtable;

static cone_vtable the_cone_vtable = {
	cone_intersect,
	cone_normalAt
};

void cone_construct_default(cone *c)
{
	shape_construct(&c->base, mat4Identity(), createMaterial(createVec3f(0.8f, 1.f, 0.6f), 0.1, 0.7, 0.2, 200.f, 0.f, 0.0f, 1.0f));
	c->base.vptr = (shape_vtable*)&the_cone_vtable;
	c->min = -(FLT_MIN-1);
	c->max = FLT_MAX;
	c->closed = 0;
}

void cone_construct(cone *c, float min, float max, uint8 closed, mat4 transform, material material, vec4f position)
{
	shape_construct(&c->base, transform, material);
	c->base.vptr = (shape_vtable*)&the_cone_vtable;
	c->min = min;
	c->max = max;
	c->closed = closed;
}

void coneSetTransform(cone *c, mat4 m)
{
	shape_setTransform((shape*)c, m);
}

void coneSetMaterial(cone *c, material m)
{
	shape_setMaterial((shape*)c, m);
}

void coneSetPattern(cone *c, pattern *pn)
{
	pattern p = *pn;
	materialSetPattern(&c->base.material, p);
}

internal uint8 checkCap(ray r, float t, float trunc)
{
	trunc = fabsf(trunc);

	float x = r.origin.x + t*r.direction.x;
	float z = r.origin.z + t*r.direction.z;

	//return (x*x + z*z) < trunc;

	if ((x*x + z*z) <= trunc + EPSILON || (x*x + z*z) <= trunc - EPSILON)
		return true;

	return false;
}

internal void intersectCaps(cone *c, intersections *is, ray r)
{
	if (c->closed == false || fabsf(r.direction.y) <= EPSILON)
		return;

	float t = (c->min - r.origin.y) / r.direction.y;
	if (checkCap(r, t, c->min))
		addIntersection(is, createIntersection(t, (shape*)c));

	t = (c->max - r.origin.y) / r.direction.y;
	if (checkCap(r, t, c->max))
		addIntersection(is, createIntersection(t, (shape*)c));
}

int cone_intersect(cone *c, intersections *is, ray r)
{
	intersectCaps(c, is, r);

	float rdx2 = r.direction.x * r.direction.x;
	float rdy2 = r.direction.y * r.direction.y;
	float rdz2 = r.direction.z * r.direction.z;

	float a = rdx2 - rdy2 + rdz2;
	float b = (2 * r.origin.x * r.direction.x) - (2 * r.origin.y * r.direction.y) + (2 * r.origin.z * r.direction.z);
	float cc = r.origin.x*r.origin.x - r.origin.y*r.origin.y + r.origin.z*r.origin.z;

	if (fabsf(a) <= EPSILON)
	{
		if (fabsf(b) <= EPSILON)
		{
			return 0;
		}

		float t = -cc / (2 * b);
		addIntersection(is, createIntersection(t, (shape*)c));
		return 1;
	}

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

//int cone_intersect(cone *c, intersections *is, ray r)
//{
//	intersectCaps(c, is, r);
//
//	double rdx2 = r.direction.x * r.direction.x;
//	double rdy2 = r.direction.y * r.direction.y;
//	double rdz2 = r.direction.z * r.direction.z;
//
//	double a = rdx2 - rdy2 + rdz2;
//	double b = (2 * (double)r.origin.x * (double)r.direction.x) - (2 * (double)r.origin.y * (double)r.direction.y) + (2 * (double)r.origin.z * (double)r.direction.z);
//	double cc = (double)r.origin.x*(double)r.origin.x - (double)r.origin.y*(double)r.origin.y + (double)r.origin.z*(double)r.origin.z;
//
//	if (fabsf(a) <= EPSILON)
//	{
//		if (fabsf(b) <= EPSILON)
//		{
//			return 0;
//		}
//
//		float t = -cc / (2 * b);
//		addIntersection(is, createIntersection(t, (shape*)c));
//		return 1;
//	}
//
//	double d = b*b - 4 * a*cc;
//	if (d < 0)
//		return 0;
//
//	double t0 = (-b - sqrt(d)) / (2 * a);
//	double t1 = (-b + sqrt(d)) / (2 * a);
//
//	if (t0 > t1)
//		swap2double(t0, t1);
//
//	double y0 = r.origin.y + t0*r.direction.y;
//	if (c->min < y0 && y0 < c->max)
//		addIntersection(is, createIntersection(t0, (shape*)c));
//
//	double y1 = r.origin.y + t1*r.direction.y;
//	if (c->min < y1 && y1 < c->max)
//		addIntersection(is, createIntersection(t1, (shape*)c));
//
//	return 1;
//}

vec4f cone_normalAt(cone *c, vec4f *point)
{
	float distance = point->x*point->x + point->z*point->z;

	if (distance < 1 && point->y >= c->max - EPSILON)
		return vec4fVector(0, 1, 0);
	else if (distance < 1 && point->y <= c->min + EPSILON)
		return vec4fVector(0, -1, 0);

	float y = sqrtf(distance);
	if (point->y > 0)
		y = -y;

	return vec4fVector(point->x, y, point->z);
}