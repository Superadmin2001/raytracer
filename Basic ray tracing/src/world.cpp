#include "world.h"

#include <vector>
#include "defines.h"

//world worldCreateDefult()
//{
//	world result;
//	result.lightCount = 1;
//	result.lights = (light*)malloc(sizeof(light)*result.lightCount);
//	result.lights[0] = createPointLight(vec4fPoint(-10, 10, -10), createVec3f(1, 1, 1));
//	result.shapeCount = 2;
//	result.shapes = (shape**)malloc(sizeof(shape*)*result.shapeCount);
//
//	sphere *s = (sphere*)malloc(sizeof(sphere));
//	sphere_construct_default(s);
//
//	sphere *inner = (sphere*)malloc(sizeof(sphere));
//	sphere_construct_default(inner);
//	sphereSetTransform(inner, mat4Scaling(vec4fVector(0.5, 0.5, 0.5)));
//
//	result.shapes[0] = (shape*)s;
//	result.shapes[1] = (shape*)inner;
//
//	return result;
//}

world worldCreate(light *lights, uint32 lightCount, shape **shapes, uint32 shapeCount)
{
	world result;
	result.lightCount = lightCount;
	result.lights = lights;
	result.shapeCount = shapeCount;
	result.shapes = shapes;

	return result;
}

uint8 worldAddSphere(world *w, shape *s)
{
	shape **sp = (shape**)realloc(w->shapes, sizeof(shape*) * (w->shapeCount + 1));

	if (!sp)
		return 0;

	w->shapes = sp;
	w->shapeCount++;
	w->shapes[w->shapeCount - 1] = s;

	return 1;
}

intersections worldIntersect(world w, ray r)
{
	intersections result = createIntersections();

	for (uint32 i = 0; i < w.shapeCount; ++i)
		intersect(w.shapes[i], &result, r);
	return result;
}

comps prepareComputations(intersection i, ray r, intersections *is)
{
	comps result;

	result.t = i.t;
	result.object = i.object;
	result.point = positionByParam(r, i.t);
	result.eyev = vec4fNegate(r.direction);
	result.normalv = normalAt(result.object, &result.point);
	result.reflectv = reflect(r.direction, result.normalv);

	if (vec4fDot(result.normalv, result.eyev) < 0)
	{
		result.inside = 1;
		result.normalv = vec4fNegate(result.normalv);
	}
	else
	{
		result.inside = 0;
	}

	result.overPoint = vec4fAdd(vec4fMulByVal(result.normalv, /*FLT_EPSILON * 15000*/EPSILON), result.point);
	result.underPoint = vec4fSub(result.point, vec4fMulByVal(result.normalv, /*FLT_EPSILON * 15000*/EPSILON));

	// TODO: rewrite in C!
	std::vector<shape*> containers;
	for (uint32 i = 0; i < is->occupied; i++)
	{
		if (is->data[i].t == result.t)
		{
			if (containers.empty())
				result.n1 = 1.0f;
			else
				result.n1 = containers[containers.size() - 1]->material.refractiveIndex;
		}
		
		auto it = std::find(containers.begin(), containers.end(), is->data[i].object);
		if (it != containers.end())
			containers.erase(it);
		else
			containers.push_back(is->data[i].object);

		if (is->data[i].t == result.t)
		{
			if (containers.empty())
				result.n2 = 1.0f;
			else
				result.n2 = containers[containers.size() - 1]->material.refractiveIndex;

			break;
		}
	}

	return result;
}

uint8 isShadowed(world w, vec4f point)
{
	vec4f toLight = vec4fSub(w.lights->position, point);
	float distance = vec4fLength(toLight);
	vec4fNormalize(&toLight);

	ray r = createRay(point, toLight);
	intersections xs = worldIntersect(w, r);

	intersection i;
	if (hit(xs, &i))
	{
		if (i.t < distance)
		{
			intersectionsCleanUp(&xs);
			return 1;
		}
	}

	intersectionsCleanUp(&xs);
	return 0;
}

vec3f worldShade(world *w, comps *c, int32 remaining)
{
	vec3f col = createVec3f(0.0f, 0.0f, 0.0f);

	for (uint32 i = 0; i < w->lightCount; i++)
	{
		uint8 isInShadow = isShadowed(*w, c->overPoint);
		vec3f temp = lighting(c->object->material, c->object, w->lights[i], c->overPoint, c->eyev, c->normalv, isInShadow);
		vec3f reflected = reflectedColor(w, c, remaining);
		vec3f refracted = refractedColor(w, c, remaining);

		material *m = &c->object->material;
		if (m->reflective > 0 && m->transparency > 0)
		{
			float reflectence = schlick(c);
			col = vec3fAdd(col, temp);
			col = vec3fAdd(col, vec3fMulByVal(reflected, reflectence));
			col = vec3fAdd(col, vec3fMulByVal(reflected, 1.0f - reflectence));
		}
		else
		{
			col = vec3fAdd(col, temp);
			col = vec3fAdd(col, reflected);
			col = vec3fAdd(col, refracted);
		}
	}

	return col;
}

vec3f worldColorAt(world w, ray r, int32 remaining)
{
	intersections xs = worldIntersect(w, r);
	intersection i;

	if (hit(xs, &i))
	{
		comps c = prepareComputations(i, r, &xs);
		intersectionsCleanUp(&xs);
		return worldShade(&w, &c, remaining);
	}

	intersectionsCleanUp(&xs);
	return createVec3f(0.0f, 0.0f, 0.0f);
}

void worldDestroy(world *w)
{
	free(w->lights);
	free(w->shapes);
}

vec4f normalAt(shape *shape, vec4f *worldPoint)
{
	vec4f localPoint = shapeWorldToObject(shape, worldPoint);
	vec4f localNormal = shape->vptr->normalAt(shape, &localPoint);
	vec4f worldNormal = shapeNormalToWorld(shape, &localNormal);

	return worldNormal;
}

vec4f normalAt_depr(shape *shape, vec4f *worldPoint)
{
	vec4f localPoint = mat4MulByVec4f(&mat4Inverse(&shape->transform), worldPoint);
	vec4f localNormal = shape->vptr->normalAt(shape, &localPoint);
	vec4f worldNormal = mat4MulByVec4f(&mat4Transpose(&mat4Inverse(&shape->transform)), &localNormal);
	worldNormal.w = 0;
	vec4fNormalize(&worldNormal);

	return worldNormal;
}