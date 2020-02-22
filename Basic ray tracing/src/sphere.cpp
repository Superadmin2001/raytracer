#include "sphere.h"
#include "intersection.h"

void sphereSetTransform(sphere *s, mat4 m)
{
	shape_setTransform((shape*)s, m);
}

void sphereSetMaterial(sphere *s, material m)
{
	shape_setMaterial((shape*)s, m);
}

void sphereSetPattern(sphere *s, pattern *pn)
{
	pattern p = *pn;
	materialSetPattern(&s->base.material, p);
}

int sphere_intersect(sphere *s, intersections *is, ray r)
{
	vec4f sphereToRay = vec4fSub(r.origin, s->position);

	float a = vec4fDot(r.direction, r.direction);
	float b = 2 * vec4fDot(r.direction, sphereToRay);
	float c = vec4fDot(sphereToRay, sphereToRay) - s->radius;
	float d = b*b - 4 * a * c;

	if (d < 0)
		return 0;

	float t1 = (-b - sqrtf(d)) / (2 * a);
	float t2 = (-b + sqrtf(d)) / (2 * a);

	intersection i[2];
	i[0].t = t1;
	i[0].object = (shape*)s;
	i[1].t = t2;
	i[1].object = (shape*)s;

	addIntersection(is, i[0]);
	addIntersection(is, i[1]);

	return 1;
}

vec4f sphere_normalAt(sphere *s, vec4f *point)
{
	//vec4f objectPoint = mat4MulByVec4f(&mat4Inverse(&s->base.transform), point);
	vec4f localNormal = vec4fSub(*point, vec4fPoint(0, 0, 0));
	/*vec4f worldNormal = mat4MulByVec4f(&mat4Transpose(&mat4Inverse(&s->base.transform)), &objectNormal);
	worldNormal.w = 0;
	vec4fNormalize(&worldNormal);*/

	//return worldNormal;
	return localNormal;
}

typedef struct sphere_vtable
{
	int(*intersect)(sphere *s, intersections *is, ray r);
	vec4f(*normalAt)(sphere *s, vec4f *worldPoint);
} sphere_vtable;

static sphere_vtable the_sphere_vtable = {
	sphere_intersect,
	sphere_normalAt
};

void sphere_construct_default(sphere *s)
{
	shape_construct(&s->base, mat4Identity(), createMaterial(createVec3f(0.8f, 1.f, 0.6f), 0.1, 0.7, 0.2, 200.f, 0.f, 0.0f, 1.0f));
	s->base.vptr = (shape_vtable*)&the_sphere_vtable;
	s->position = vec4fPoint(0, 0, 0);
	s->radius = 1;
}

void sphere_construct(sphere *s, mat4 transform, material material, vec4f position, float radius)
{
	shape_construct(&s->base, transform, material);
	s->base.vptr = (shape_vtable*)&the_sphere_vtable;
	s->position = position;
	s->radius = radius;
}

void sphereCreateGlass(sphere *s, float refractiveIndex)
{
	shape_construct(&s->base, mat4Identity(), createMaterial(createVec3f(0.8f, 1.f, 0.6f), 0.1, 0.7, 0.2, 200.f, 0.f, 0.0f, 1.0f));
	s->base.vptr = (shape_vtable*)&the_sphere_vtable;
	s->position = vec4fPoint(0, 0, 0);
	s->radius = 1;
	s->base.material.transparency = 1.0f;
	s->base.material.refractiveIndex = refractiveIndex;
}