#include "group.h"
#include "../defines.h"
#include "../intersection.h"

typedef struct group_vtable
{
	int(*intersect)(group *g, intersections *is, ray r);
	vec4f(*normalAt)(group *g, vec4f *worldPoint);
} group_vtable;

static group_vtable the_group_vtable = {
	groupIntersect,
	groupNormalAt
};

void groupConstructDefault(group *g)
{
	shape_construct(&g->base, mat4Identity(), createDeafultMaterial());
	g->base.vptr = (shape_vtable*)&the_group_vtable;
	g->capacity = 4;
	g->count = 0;
	g->shapes = (shape**)malloc(sizeof(shape*) * g->capacity);
}

int groupIntersect(group *g, intersections *is, ray r)
{
	uint32 count = g->count;
	uint8 isHited = false;

	ray localRay;
	for (uint32 i = 0; i < count; i++)
	{
		localRay = rayTransform(r, &mat4Inverse(&g->base.transform));
		isHited = intersect(g->shapes[i], is, localRay);
		if (isHited == true)
			isHited == true;
	}

	return isHited;
}

vec4f groupNormalAt(group *g, vec4f *worldPoint)
{
	return { 0.0, 0.0, 0.0, 0.0 };
}

void groupSetTransform(group *g, mat4 m)
{
	shape_setTransform((shape*)g, m);
}

void groupSetMaterial(group *g, material m)
{
	shape_setMaterial((shape*)g, m);
}

void groupSetPattern(group *g, pattern *pn)
{
	pattern p = *pn;
	materialSetPattern(&g->base.material, p);
}

uint8 groupAddShape(group *g, shape *s)
{
	if (g->count < g->capacity)
	{
		g->shapes[g->count++] = s;
		s->parent = g;
		return true;
	}

	shape **reallocated = (shape**)realloc(g->shapes, sizeof(shape*)*g->capacity*2);
	if (reallocated)
	{
		g->shapes = reallocated;
		g->capacity *= 2;
		g->shapes[g->count++] = s;
		s->parent = g;
		return  true;
	}

	return false;
}

void groupPrintShapesAddresses(group *g)
{
	if (g->count == 0)
	{
		printf("[ ]\n");
		return;
	}

	printf("[ ");
	for (uint32 i = 0; i < g->count - 1; i++)
		printf("%p, ", (void*)g->shapes[i]);
	printf("%p ]\n", (void*)g->shapes[g->count - 1]);
}