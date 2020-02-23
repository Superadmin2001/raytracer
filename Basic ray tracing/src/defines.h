#ifndef DEFINES_H
#define DEFINES_H

#define internal static

#include <float.h>
#define EPSILON (FLT_EPSILON * 15000)

#define swapVec2fComponents(v) { float temp = v.x; v.x = v.y; v.y = temp; }
#define swap2f(v1, v2) { float temp = v1; v1 = v2; v2 = temp; }

#define setSign(v) { (v > 0.0f) ? v = 1.0f : v = -1.0f; }

#define true 1
#define false 0

#endif

