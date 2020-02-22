#ifndef DEFINES_H
#define DEFINES_H

#define internal static

#include <float.h>
#define EPSILON (FLT_EPSILON * 15000)

#define swapVec2fComponents(v) { float temp = v.x; v.x = v.y; v.y = temp; }

#define setSign(v) { (v > 0.0f) ? v = 1.0f : v = -1.0f; }

#endif

