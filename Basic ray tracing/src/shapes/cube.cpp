#include "cube.h"

#include "../defines.h"

#include "../intersection.h"

typedef struct cube_vtable
{
	int(*intersect)(cube *c, intersections *is, ray r);
	vec4f(*normalAt)(cube *c, vec4f *worldPoint);
} cube_vtable;

static cube_vtable the_cube_vtable = {
	cube_intersect,
	cube_normalAt
};

void cube_construct_default(cube *c)
{
	shape_construct(&c->base, mat4Identity(), createMaterial(createVec3f(0.8f, 1.f, 0.6f), 0.1, 0.7, 0.2, 200.f, 0.f, 0.0f, 1.0f));
	c->base.vptr = (shape_vtable*)&the_cube_vtable;
	c->position = vec4fPoint(0, 0, 0);
}

void cube_construct(cube *c, mat4 transform, material material, vec4f position)
{
	shape_construct(&c->base, transform, material);
	c->base.vptr = (shape_vtable*)&the_cube_vtable;
	c->position = position;
}

void cubeSetTransform(cube *c, mat4 m)
{
	shape_setTransform((shape*)c, m);
}

void cubeSetMaterial(cube *c, material m)
{
	shape_setMaterial((shape*)c, m);
}

void cubeSetPattern(cube *c, pattern *pn)
{
	pattern p = *pn;
	materialSetPattern(&c->base.material, p);
}

internal vec2f checkAxis(float origin, float direction)
{
	float tMinNumerator = -1 - origin;
	float tMaxNumerator = 1 - origin;

	vec2f t;
	if (fabsf(direction) >= EPSILON)
	{
		t.x = tMinNumerator / direction;
		t.y = tMaxNumerator / direction;
	}
	else
	{
		setSign(tMinNumerator);
		setSign(tMaxNumerator);
		t.x = FLT_MAX * tMinNumerator;
		t.y = FLT_MAX * tMaxNumerator;
	}

	if (t.x > t.y)
		swapVec2fComponents(t);

	return t;
}

int cube_intersect(cube *c, intersections *is, ray r)
{
	vec2f x = checkAxis(r.origin.x, r.direction.x);
	vec2f y = checkAxis(r.origin.y, r.direction.y);
	vec2f z = checkAxis(r.origin.z, r.direction.z);

	float tmin = max3f(x.x, y.x, z.x);
	float tmax = min3f(x.y, y.y, z.y);

	if (tmin > tmax)
		return 0;

	intersection i[2];
	i[0] = createIntersection(tmin, (shape*)c);
	i[1] = createIntersection(tmax, (shape*)c);

	addIntersection(is, i[0]);
	addIntersection(is, i[1]);

	return 1;
}

vec4f cube_normalAt(cube *c, vec4f *point)
{
	float absX = fabsf(point->x);
	float absY = fabsf(point->y);
	float absZ = fabsf(point->z);

	float maxc = max3f(absX, absY, absZ);

	if (maxc == absX)
		return vec4fVector(point->x, 0, 0);
	else if (maxc == absY)
		return vec4fVector(0, point->y, 0);

	return vec4fVector(0, 0, point->z);
}