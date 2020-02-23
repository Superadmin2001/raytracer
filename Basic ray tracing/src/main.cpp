#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb\stb_image_write.h>

#include <string.h>

#include "rt.h"

void testMat()
{
	mat4 a;

	mat4SetElement(&a, 0, 0, 9);
	mat4SetElement(&a, 0, 1, 3);
	mat4SetElement(&a, 0, 2, 0);
	mat4SetElement(&a, 0, 3, 9);

	mat4SetElement(&a, 1, 0, -5);
	mat4SetElement(&a, 1, 1, -2);
	mat4SetElement(&a, 1, 2, -6);
	mat4SetElement(&a, 1, 3, -3);

	mat4SetElement(&a, 2, 0, -4);
	mat4SetElement(&a, 2, 1, 9);
	mat4SetElement(&a, 2, 2, 6);
	mat4SetElement(&a, 2, 3, 4);

	mat4SetElement(&a, 3, 0, -7);
	mat4SetElement(&a, 3, 1, 6);
	mat4SetElement(&a, 3, 2, 6);
	mat4SetElement(&a, 3, 3, 2);

	mat4Print(&a);
	
	/*mat4 matOfCofactors = mat4Inverse(&a);
	mat4Print(&matOfCofactors);

	mat4 inverse = mat4Transpose(&matOfCofactors);
	mat4Print(&inverse);

	float determinant = mat4Determinant(&a);
	for (uint8 i = 0; i < 16; i++)
		inverse.data[i] /= determinant;

	mat4Print(&inverse);*/

	mat4 inversed = mat4Inverse(&a);
	mat4Print(&inversed);
}

void setPixel_new(int x, int y, color c, uint32 width, uint32 height, uint8 *pixels)
{
	int size = width * height * 3;
	int index = (y * width + x) * 3;

	if (index < size - 3&& index >= 0)
	{
		pixels[index] = c.r;
		pixels[index + 1] = c.g;
		pixels[index + 2] = c.b;
	}
}

color* drawClock(uint32 width, uint32 height)
{
	color *buffer = (color*)malloc(width * height * sizeof(color));
	memset(buffer, 0, width * height * sizeof(color));

	color white = { 255, 255, 255 };

	mat4 rotation = mat4RotationZ(-M_PI / 6);
	vec4f point = vec4fPoint(width / 4, 0, 0), pointTemp;
	for (uint8 i = 0; i < 12; i++, point = mat4MulByVec4f(&rotation, &point))
	{
		pointTemp = { point.x + width / 2, point.y + height / 2, point.z, point.w };
		setPixel_new(pointTemp.x, pointTemp.y, white, width, height, ((uint8*)buffer));
	}

	return buffer;
}

void writeToPPM(uint8 *buffer, uint32 width, uint32 height, const char *filename)
{
	FILE *image = fopen(filename, "w");

	fprintf(image, "P6\n%u %u\n255\n", width, height);
	uint32 count = width*height * 3;

	for (uint32 i = 0; i < count-2; i++)
		fprintf(image, "%u %u %u\n", buffer[i], buffer[i+1], buffer[i+2]);

	//fwrite(buffer, sizeof(uint8), count, image);

	fclose(image);
}

void trace()
{
	int width = 300, height = 300;
	color *image = (color*)malloc(width * height * sizeof(color));
	memset(image, 0, width * height * sizeof(color));
	//rtRender(image, width, height);
	stbi_write_jpg("sphere.jpg", width, height, 3, image, 100);
}

int main()
{
	//render();
#ifdef _DEBUG
	//_CrtDumpMemoryLeaks();
#endif
	//rtTest();
	test();
	return(0);
}