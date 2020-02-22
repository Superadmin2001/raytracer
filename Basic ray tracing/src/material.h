#ifndef MATERIAL_H
#define MATERIAL_H

#include "math\geometry.h"

#include "pattern\pattern.h"

typedef struct material
{
	vec3f color;
	pattern pattern;
	uint8 isPatternExists = 0;
	float ambient;
	float diffuse;
	float specular;
	float shininess;
	float reflective;
	float transparency;
	float refractiveIndex;
} material;

material createDeafultMaterial();
material createMaterial(vec3f color, float ambient, float diffuse, float specular, float shininess, float reflective, float transparency, float refractiveIndex);

void materialSetPattern(material *m, pattern pn);
void materialRemovePattern(material *m);

#endif 