#include "light.h"
#include "world.h"

light createPointLight(vec4f position, vec3f intensity)
{
	light pl;
	pl.position = position;
	pl.intensity = intensity;

	return pl;
}

vec4f reflect(vec4f in, vec4f normal)
{
	return vec4fSub(in, vec4fMulByVal(vec4fMulByVal(normal, 2), vec4fDot(in, normal)));
}

vec3f reflectedColor(world *w, comps *c, int32 remaining)
{
	if (c->object->material.reflective == 0 || remaining < 0)
		return createVec3f(0, 0, 0);

	ray reflectedRay = createRay(c->overPoint, c->reflectv);

	vec3f col = worldColorAt(*w, reflectedRay, remaining-1);

	return vec3fMulByVal(col, c->object->material.reflective);
}

uint8 checkForTotalInternalReflection(comps *c, vec4f *direction)
{
	float nRatio = c->n1 / c->n2;
	float cosI = vec4fDot(c->eyev, c->normalv);
	float sin2T = (nRatio *nRatio) * (1 - cosI * cosI);

	if (sin2T > 1)
	{
		return 1;
	}


	float cosT = sqrtf(1.0 - sin2T);
	float t = (nRatio * cosI - cosT);
	*direction = vec4fSub(vec4fMulByVal(c->normalv, t), vec4fMulByVal(c->eyev, nRatio));
	return 0;
}

vec3f refractedColor(world *w, comps *c, int32 remaining)
{
	vec4f direction;
	uint8 f = checkForTotalInternalReflection(c, &direction);
	if (c->object->material.transparency == 0 || remaining == 0 || f)
		return createVec3f(0, 0, 0);

	ray refractedRay = createRay(c->underPoint, direction);
	vec3f col = vec3fMulByVal(worldColorAt(*w, refractedRay, remaining - 1), c->object->material.transparency);

	return col;
}

float schlick(comps *c)
{
	float cos = vec4fDot(c->eyev, c->normalv);

	if (c->n1 > c->n2)
	{
		float n = c->n1 / c->n2;
		float sin2T = n*n * (1.0f - cos*cos);

		if (sin2T > 1.0f)
			return 1.0f;

		float cos2T = sqrtf(1.0f - sin2T);
		cos = cos2T;
	}

	float r0 = powf(((c->n1 - c->n2) / (c->n1 + c->n2)), 2.0f);
	return r0 + (1 - r0) * powf((1-cos), 5.0);
}

vec3f lighting(material m, shape *object, light l, vec4f point, vec4f eyev, vec4f normalv, uint8 isShadowed)
{
	vec3f col;
	if (m.isPatternExists)
		col = patternAtShape(&m.pattern, object, &point);
	else
		col = m.color;

	vec3f effectiveColor = vec3fMul(col, l.intensity);
	vec3f ambient = vec3fMulByVal(effectiveColor, m.ambient);

	if (isShadowed)
		return ambient;

	vec4f lightv = vec4fSub(l.position, point);
	vec4fNormalize(&lightv);

	vec3f diffuse, specular;

	float lightDotNormal = vec4fDot(lightv, normalv);
	if (lightDotNormal < 0)
	{
		diffuse = { 0, 0, 0 };
		specular = { 0, 0, 0 };
	}
	else
	{
		diffuse = vec3fMulByVal(effectiveColor, m.diffuse * lightDotNormal);

		vec4f reflectv = reflect(vec4fNegate(lightv), normalv);
		float reflectDotEye = vec4fDot(reflectv, eyev);

		if (reflectDotEye <= 0)
		{
			specular = { 0, 0, 0 };
		}
		else
		{
			float factor = powf(reflectDotEye, m.shininess);
			specular = vec3fMulByVal(l.intensity, m.specular * factor);
		}
	}

	return vec3fAdd(vec3fAdd(ambient, diffuse), specular);
}