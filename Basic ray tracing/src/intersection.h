#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "math\utils.h"
#include "ray.h"
#include "shapes\shape.h"

#include <stdio.h>

typedef struct intersection
{
	float t;
	shape *object;
} intersection;

typedef struct intersections
{
	uint32 count;
	uint32 occupied;
	intersection *data;
} intersections;

intersection createIntersection(float t, shape *object);
intersections createIntersections();

inline uint8 getIntersectedObjectType(intersection i);

int addIntersection(intersections *is, intersection i);
int intersect(shape *shape, intersections *is, ray r);
int cmpFunc(const void *a, const void *b);
int hit(intersections is, intersection *i);

void printIntersections(intersections *is);
void intersectionsCleanUp(intersections *xs);

#endif 