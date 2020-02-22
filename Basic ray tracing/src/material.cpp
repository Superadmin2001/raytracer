#include "material.h"

material createMaterial(vec3f color, float ambient, float diffuse, float specular, float shininess, float reflective, float transparency, float refractiveIndex)
{
	material result;
	result.color = color;
	result.ambient = ambient;
	result.diffuse = diffuse;
	result.specular = specular;
	result.shininess = shininess;
	result.reflective = reflective;
	result.transparency = transparency;
	result.refractiveIndex = refractiveIndex;
	return result;
}

material createDeafultMaterial()
{
	//return createMaterial(createVec3f(0.8f, 1.f, 0.6f), 0.1, 0.7, 0.2, 200.f);
	return createMaterial(createVec3f(1.f, 1.f, 1.f), 0.1, 0.7, 0.2, 200.f, 0.0f, 0.0f, 1.0f);
}

void materialSetPattern(material *m, pattern pn)
{
	m->pattern = pn;
	m->isPatternExists = 1;
}

void materialRemovePattern(material *m)
{
	m->isPatternExists = 0;
}