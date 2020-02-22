#ifndef LIGHT_H
#define LIGHT_H

#include "math\geometry.h"
#include "material.h"
#include "math\utils.h"
#include "sphere.h"

typedef struct world world;
typedef struct comps comps;

typedef struct light
{
	vec4f position;
	vec3f intensity;
} light;

light createPointLight(vec4f position, vec3f intensity);

vec4f reflect(vec4f in, vec4f normal);
vec3f reflectedColor(world *w, comps *c, int32 remaining);
uint8 checkForTotalInternalReflection(comps *c, vec4f *direction);
vec3f refractedColor(world *w, comps *c, int32 remaining);

float schlick(comps *c);

vec3f lighting(material m, shape *object, light l, vec4f point, vec4f eyev, vec4f normalv, uint8 isShadowed);

#endif