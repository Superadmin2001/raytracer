#ifndef SPHERE_H
#define SPHERE_H

#include "shape.h"

typedef struct sphere
{
	shape base;
	vec4f position;
	float radius;
} sphere;

void sphere_construct_default(sphere *s);
void sphere_construct(sphere *s, mat4 transform, material material, vec4f position, float radius);
void sphereCreateGlass(sphere *s, float refractiveIndex);

void sphereSetTransform(sphere *s, mat4 m);
void sphereSetMaterial(sphere *s, material m);
void sphereSetPattern(sphere *s, pattern *pn);

vec4f sphere_normalAt(sphere *s, vec4f *point);

int sphere_intersect(sphere *s, intersections *is, ray r);

#endif