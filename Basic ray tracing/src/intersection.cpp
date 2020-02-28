#include "intersection.h"
#include "defines.h"

intersection createIntersection(float t, shape *object)
{
	intersection result;
	result.t = t;
	result.object = object;

	return result;
}

inline uint8 getIntersectedObjectType(intersection i)
{
	return (*(uint8*)i.object);
}

intersections createIntersections()
{
	intersections result;
	result.count = 8;
	result.occupied = 0;
	result.data = (intersection*)malloc(sizeof(intersection) * result.count);

	return result;
}

void printIntersections(intersections *is)
{
	if (is->occupied == 0)
	{
		printf("[ ]\n");
		return;
	}

	printf("[ ");
	for (uint32 i = 0; i < is->occupied - 1; i++)
		printf("%f(%p), ", is->data[i].t, is->data[i].object);
	printf("%f(%p) ]\n", is->data[is->occupied - 1].t, is->data[is->occupied - 1].object);
}

internal void sortAdd(intersections *is, intersection i)
{
	is->data[is->occupied++] = i;

	if (is->occupied == 1)
		return;

	intersection temp;
	for (uint32 c1 = 0; c1 < is->occupied; c1++)
	{
		for (uint32 c2 = 0; c2 < is->occupied - 1; c2++)
		{
			if (is->data[c2].t > is->data[c2 + 1].t)
			{
				temp = is->data[c2];
				is->data[c2] = is->data[c2 + 1];
				is->data[c2 + 1] = temp;
			}
		}
	}
}

int addIntersection(intersections *is, intersection i)
{
	if (is->occupied >= is->count)
	{
		void *reallocated = realloc(is->data, sizeof(intersection)*is->count * 2);
		if (reallocated)
		{
			is->data = (intersection*)reallocated;
			is->count *= 2;
			sortAdd(is, i);
			return true;
		}

		return false;
	}

	sortAdd(is, i);
	return true;
}

int addIntersection_depr(intersections *is, intersection i)
{
	if (is->occupied < is->count)
	{
		is->data[is->occupied++] = i;

		return 1;
	}
	else
	{
		void *reallocated = realloc(is->data, sizeof(intersection)*is->count * 2);
		if (reallocated)
		{
			is->data = (intersection*)reallocated;
			is->count *= 2;
			is->data[is->occupied] = i;
			is->occupied++;

			return 1;
		}

		return 0;
	}
}

int intersect(shape *shape, intersections *is, ray r)
{
	ray localRay = rayTransform(r, &mat4Inverse(&shape->transform));
	return shape->vptr->intersect(shape, is, localRay);
}

void printIntersections(intersections is)
{
	for (uint32 i = 0; i < is.occupied; i++)
		printf("(%u) [ t = %5f, object = %p ]\n", i, is.data[i].t, is.data[i].object);
}

int cmpFunc(const void *a, const void *b)
{
	return((*(intersection*)a).t >(*(intersection*)b).t);
}

int hit(intersections is, intersection *i)
{
	qsort(is.data, is.occupied, sizeof(intersection), cmpFunc);

	for (uint32 c = 0; c < is.occupied; c++)
	{
		if (is.data[c].t >= 0)
		{
			*i = is.data[c];
			return 1;
		}
	}

	return 0;
}

void intersectionsCleanUp(intersections *xs)
{
	free(xs->data);
}