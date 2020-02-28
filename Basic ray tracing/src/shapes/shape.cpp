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
	s->parent = NULL;
}

void shape_setMaterial(shape *s, material material)
{
	s->material = material;
}

void shape_setTransform(shape *s, mat4 transform)
{
	s->transform = transform;
}

vec4f shapeWorldToObject(shape *s, vec4f *point)
{
	if (s->parent != NULL)
		*point = shapeWorldToObject((shape*)s->parent, point);

	return mat4MulByVec4f(&mat4Inverse(&s->transform), point);
}

vec4f shapeNormalToWorld(shape *s, vec4f *normal)
{
	*normal = mat4MulByVec4f(&mat4Transpose(&mat4Inverse(&s->transform)), normal);
	normal->w = 0;
	vec4fNormalize(normal);

	if (s->parent != NULL)
		*normal = shapeNormalToWorld((shape*)s->parent, normal);

	return *normal;
}