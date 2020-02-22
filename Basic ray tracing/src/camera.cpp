#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "camera.h"

camera camCreate(uint32 width, uint32 height, float fieldOfView)
{
	camera result;
	result.width = width;
	result.height = height;
	result.fieldOfView = fieldOfView;
	result.transform = mat4Identity();

	float halfView = tanf(fieldOfView / 2);
	float aspectRatio = width / height;

	if (aspectRatio >= 1)
	{
		result.halfWidth = halfView;
		result.halfHeight = halfView / aspectRatio;
	}
	else
	{
		result.halfWidth = halfView * aspectRatio;
		result.halfHeight = halfView;
	}

	result.pixelSize = (result.halfWidth * 2) / result.width;

	return result;
}

void camSetTransform(camera *cam, mat4 t)
{
	cam->transform = t;
}

mat4 camViewTransform(vec4f from, vec4f to, vec4f up)
{
	vec4fNormalize(&up);

	vec4f forward = vec4fSub(to, from);
	vec4fNormalize(&forward);

	vec4f left = vec4fCross(forward, up);

	vec4f trueUp = vec4fCross(left, forward); // allows to up vector be precisely approximately up

	mat4 result = mat4Identity();

	mat4SetElement(&result, 0, 0, left.x);
	mat4SetElement(&result, 0, 1, left.y);
	mat4SetElement(&result, 0, 2, left.z);
	mat4SetElement(&result, 1, 0, trueUp.x);
	mat4SetElement(&result, 1, 1, trueUp.y);
	mat4SetElement(&result, 1, 2, trueUp.z);
	mat4SetElement(&result, 2, 0, -forward.x);
	mat4SetElement(&result, 2, 1, -forward.y);
	mat4SetElement(&result, 2, 2, -forward.z);

	mat4 translation = mat4Translation(vec4fVector(-from.x, -from.y, -from.z));
	result = mat4Mul(&result, &translation);

	return result;
}

ray camRayForPixel(camera *cam, uint32 x, uint32 y)
{
	vec2f offset = { (x + 0.5f) * cam->pixelSize,  (y + 0.5f) * cam->pixelSize };

	vec2f world = { cam->halfWidth - offset.x, cam->halfHeight - offset.y };

	float wallPos = -1;

	mat4 inversed = mat4Inverse(&cam->transform);
	vec4f pixel = mat4MulByVec4f(&inversed, &vec4fPoint(world.x, world.y, wallPos));
	vec4f origin = mat4MulByVec4f(&inversed, &vec4fPoint(0, 0, 0));
	vec4f direction = vec4fSub(pixel, origin);
	vec4fNormalize(&direction);

	return createRay(origin, direction);
}

internal void rtSetPixel(int x, int y, color c, uint32 width, uint32 height, uint8 *pixels)
{
	int size = width * height * 3;
	int index = (y * width + x) * 3;

	if (index <= size - 3 && index >= 0)
	{
		pixels[index] = c.r;
		pixels[index + 1] = c.g;
		pixels[index + 2] = c.b;
	}
}

color* camRender(camera cam, world w)
{
	uint32 sizeOfImage = sizeof(color) * cam.width * cam.height;
	color *image = (color*)malloc(sizeOfImage);
	memset(image, 0, sizeOfImage);

	ray r;
	vec3f col;
	uint8 f = 1;
	int32 recursionLevel = 4;
	for (uint32 y = 0; y < cam.height; ++y)
	{
		for (uint32 x = 0; x < cam.width; ++x)
		{
			r = camRayForPixel(&cam, x, y);
			col = worldColorAt(w, r, recursionLevel);
			
			col = { 255 * col.x, 255 * col.y, 255 * col.z };
			rtSetPixel(x, y, vec3fToColor(col), cam.width, cam.height, (uint8*)image);
		}

		if (y % 10 == 0)
		{
			system("cls");
			float t = ((float)y / (float)cam.height) * 100;
			printf("%d%%", (int32)t);
		}
	}

	return image;
}