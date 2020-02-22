#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"
#include "defines.h"
#include "world.h"

typedef struct camera
{
	uint32 width; // in pixels
	uint32 height; // in pixels
	float  halfWidth;
	float  halfHeight;
	float  pixelSize;
	float  fieldOfView;
	mat4   transform;
} camera;

camera camCreate(uint32 width, uint32 height, float fieldOfView);

void camSetTransform(camera *cam, mat4 t);

mat4 camViewTransform(vec4f from, vec4f to, vec4f up);

ray camRayForPixel(camera *cam, uint32 x, uint32 y);

internal void rtSetPixel(int x, int y, color c, uint32 width, uint32 height, uint8 *pixels);

color* camRender(camera cam, world w);

#endif