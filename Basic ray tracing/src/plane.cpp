#include "plane.h"

#include "defines.h"
#include "intersection.h"

int plane_intersect(plane *p, intersections *is, ray r)
{
	if (fabsf(r.direction.y) < EPSILON)
		return 0;

	float t = -r.origin.y / r.direction.y;
	intersection i = createIntersection(t, (shape*)p);
	addIntersection(is, i);

	return 1;
}

vec4f plane_normalAt(plane *p, vec4f *point)
{
	return vec4fVector(0, 1, 0);
}

typedef struct plane_vtable
{
	int(*intersect)(plane *p, intersections *is, ray r);
	vec4f(*normalAt)(plane *p, vec4f *point);
} plane_vtable;

static plane_vtable the_plane_vtable = {
	plane_intersect,
	plane_normalAt
};

void plane_construct_default(plane *p)
{
	shape_construct((shape*)p, mat4Identity(), createDeafultMaterial());
	p->base.vptr = (shape_vtable*)&the_plane_vtable;
}

void plane_construct(plane *p, mat4 transform, material material)
{
	shape_construct((shape*)p, transform, material);
	p->base.vptr = (shape_vtable*)&the_plane_vtable;
}

void planeSetTransform(plane *p, mat4 transform)
{
	((shape*)p)->transform = transform;
}

void planeSetPattern(plane *p, pattern *pn)
{
	pattern patrn = *pn;
	materialSetPattern(&p->base.material, patrn);
}