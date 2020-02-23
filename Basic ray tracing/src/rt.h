#ifndef RT_H
#define RT_H

#define _USE_MATH_DEFINES
#include <float.h>

#include "defines.h"
#include "world.h"
#include "camera.h"

#include "plane.h"

#include "pattern\pattern.h"
#include "pattern\pattern_gradient.h"
#include "pattern\pattern_ring.h"
#include "pattern\pattern_chekers.h"

#include "shapes\cube.h"
#include "shapes\cylinder.h"
#include "shapes\cone.h"

internal color rtVec3fToColor(vec3f vec)
{
	color result;

	if (vec.x > 255)
		vec.x = 255;
	if (vec.y > 255)
		vec.y = 255;
	if (vec.z > 255)
		vec.z = 255;

	result = colorCreate((uint8)vec.x, (uint8)vec.y, (uint8)vec.z);
	return result;
}

#ifdef depr
void rtRender(color *buffer, int32 width, int32 height)
{
	sphere s = createSphere(vec4fPoint(0, 0, 0), 1);
	s.material.color = { 1.f, 0.2f, 1.f };

	vec4f lightPos = vec4fPoint(-10, 10, -10);
	vec3f lightColor = createVec3f(1, 1, 1);
	light l = createPointLight(lightPos, lightColor);

	vec4f rayOrigin = vec4fPoint(0, 0, -5);
	float wallZ = 10;
	float wallSize = 7;
	float half = wallSize / 2;
	float pixelSize = wallSize / ((float)width);
	vec2f worldCoords;

	world w = worldCreateDefult();

	for (int32 y = 0; y < height; y++)
	{
		worldCoords.y = half - pixelSize * y;

		for (int32 x = 0; x < width; x++)
		{
			worldCoords.x = -half + pixelSize * x;

			vec4f dir = vec4fPoint(worldCoords.x, worldCoords.y, wallZ);
			dir = vec4fSub(dir, rayOrigin);
			vec4fNormalize(&dir);

			ray r = createRay(rayOrigin, dir);

			vec3f col = worldColorAt(w, r);

			col = { 255 * col.x, 255 * col.y, 255 * col.z };
			rtSetPixel(x, y, vec3fToColor(col), width, height, (uint8*)buffer);
		}
	}
}
#endif

void render()
{
	sphere behindMiddle;
	sphere_construct_default(&behindMiddle);
	mat4 behindMiddletransform = mat4Scaling(vec4fVector(0.5f, 0.5f, 0.5f));
	behindMiddletransform = mat4Mul(&mat4Translation(vec4fVector(0.5f, 0.5f, 5)), &mat4Scaling(vec4fVector(0.5f, 0.5f, 0.5f)));
	sphereSetTransform(&behindMiddle, behindMiddletransform);

	sphere middle;
	sphere_construct_default(&middle);
	sphereSetTransform(&middle, mat4Translation(vec4fVector(-0.5, 1, 0.5)));
	middle.base.material.color = createVec3f(1, 1, 1);
	middle.base.material.diffuse = 0.0;
	middle.base.material.specular = 0.0;
	pattern_ring patternRing;
	pattern_ring_construct_default(&patternRing);
	patternSetTransform((pattern*)&patternRing, mat4Scaling(vec4fVector(0.05f, 0.05f, 0.05f)));
	//sphereSetPattern(&middle, (pattern*)&patternRing);
	middle.base.material.refractiveIndex = 1.5f;
	middle.base.material.transparency = 1.0f;

	sphere right;
	sphere_construct_default(&right);
	mat4 translation = mat4Translation(vec4fVector(1.5, 0.5, -0.5));
	mat4 scaling = mat4Scaling(vec4fVector(0.5, 0.5, 0.5));
	mat4 transform = mat4Mul(&translation, &scaling);
	sphereSetTransform(&right, transform);
	right.base.material.color = createVec3f(1.f, 1.f, 1.f); //createVec3f(0.5f, 1.f, 0.1f);
	right.base.material.diffuse = 0.0;
	right.base.material.specular = 0.3;
	right.base.material.reflective = 1.f;

	sphere left;
	sphere_construct_default(&left);
	translation = mat4Translation(vec4fVector(-1.5, 0.33+0.7, -0.75));
	scaling = mat4Scaling(vec4fVector(0.33, 0.33, 0.33));
	transform = mat4Mul(&translation, &scaling);
	sphereSetTransform(&left, transform);
	left.base.material.color = createVec3f(1.f, 0.0f, 1.0f);
	left.base.material.diffuse = 0.7;
	left.base.material.specular = 0.3;

	light l = createPointLight(vec4fPoint(-10, 10, -10), createVec3f(1, 1, 1));
	light *lights = (light*)malloc(sizeof(light));
	lights[0] = l;

	uint8 shapeCount = 8;
	shape **shapes = (shape**)malloc(sizeof(shape*) * shapeCount);
	shapes[0] = (shape*)&middle;
	shapes[1] = (shape*)&right;
	shapes[2] = (shape*)&left;

	plane pFloor, pWall, pTransparent;
	plane_construct_default(&pFloor);
	plane_construct_default(&pWall);
	plane_construct_default(&pTransparent);

	mat4 planeTransform = mat4Mul(&mat4Translation(vec4fVector(0, 0, 10)), &mat4RotationX(M_PI_2));
	planeSetTransform(&pWall, planeTransform);
	//pFloor.base.material.reflective = 1.f;

	/*pTransparent.base.material.refractiveIndex = 1.5f;
	pTransparent.base.material.transparency = 1.0f;*/
	pTransparent.base.material.color = createVec3f(0, 0, 1);
	pTransparent.base.material.diffuse = 0.0;
	pTransparent.base.material.specular = 0.0;
	pTransparent.base.material.transparency = 0.5f;
	pTransparent.base.material.reflective = 0.5f;
	pTransparent.base.material.refractiveIndex = 1.5f;
	mat4 transparentPlaneTransform = mat4Translation(vec4fVector(0, 1, 0));
	planeSetTransform(&pTransparent, transparentPlaneTransform);

	pattern pn;
	pattern_construct_default(&pn);
	pattern_checkers planeCheckers;
	pattern_checkers_construct_default(&planeCheckers);
	materialSetPattern(&pFloor.base.material, pn);
	planeSetPattern(&pWall, (pattern*)&planeCheckers);

	shapes[3] = (shape*)&pFloor;
	shapes[4] = (shape*)&pWall;
	shapes[5] = (shape*)&behindMiddle;

	cube cubeD;
	cube_construct_default(&cubeD);
	mat4 cubeTranslation = mat4Translation(vec4fVector(2, 2, 0));
	mat4 cubeScaling = mat4Scaling(vec4fVector(0.25, 0.25, 0.25));
	mat4 cubeTransform = mat4Mul(&cubeTranslation, &cubeScaling);
	cubeSetTransform(&cubeD, cubeTransform);
	shapes[6] = (shape*)&cubeD;

	cylinder cyl;
	cylinder_construct_default(&cyl);
	cyl.min = 1;
	cyl.max = 2;
	cyl.closed = true;
	mat4 cylinderRotaion = mat4RotationX(degreesToRadians(60));
	mat4 cylinderTranslation = mat4Translation(vec4fVector(2, 2, 0));
	mat4 cylinderTransform = mat4Mul(&cubeTranslation, &cylinderRotaion);
	cylinderSetTransform(&cyl, cylinderTransform);
	shapes[7] = (shape*)&cyl;

	world w = worldCreate(lights, 1, shapes, shapeCount);

	uint32 width = 1920, height = 1080;
	camera cam = camCreate(width, height, M_PI / 3);
	mat4 viewTransform = camViewTransform(vec4fPoint(0, 1.5, -5), vec4fPoint(0, 1, 0), vec4fVector(0, 1, 0));
	camSetTransform(&cam, viewTransform);

	color *image = camRender(cam, w);
	stbi_write_jpg("cube.jpg", width, height, 3, image, 100);

	worldDestroy(&w);
}	

void rtTest()
{
	sphere glassSphere;
	sphereCreateGlass(&glassSphere, 1.5f);
	ray r = createRay(vec4fVector(0, 0.99f, -2), vec4fPoint(0, 0, 1));
	intersections xs = createIntersections();
	addIntersection(&xs, createIntersection(1.8589f, (shape*)&glassSphere));
	comps cmp = prepareComputations(xs.data[0], r, &xs);
	float reflectance = schlick(&cmp);
}

void test()
{
	cone c;
	c.min = -0.5;
	c.max = 0.5;
	c.closed = true;
	vec4f point = vec4fPoint(0, 0, -0.25);
	vec4f direction = vec4fVector(0, 1, 0);
	vec4fNormalize(&direction);
	ray r = createRay(point, direction);
	intersections xs = createIntersections();
	cone_intersect(&c, &xs, r);
	printIntersections(&xs);

	/*vec4f n = cylinder_normalAt(&c, &point);
	vec4fPrint(&n);*/
}

#endif 
