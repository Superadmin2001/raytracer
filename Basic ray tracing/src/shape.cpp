#include "shape.h"

int shape_intersect(shape *s, intersections *is, ray r)
{
	return 0;
}

vec4f shape_normalAt(shape *s, vec4f *point)
{
	return vec4fVector(0, 0, 0);
}

static shape_vtable the_shape_vtable = {
	shape_intersect,
	shape_normalAt
};

void shape_construct(shape *s, mat4 transform, material material)
{
	s->vptr = &the_shape_vtable;
	s->transform = transform;
	s->material = material;
}

void shape_setMaterial(shape *s, material material)
{
	s->material = material;
}

void shape_setTransform(shape *s, mat4 transform)
{
	s->transform = transform;
}