#ifndef GFX_MATH_H
#define GFX_MATH_H

#include <math.h>

#define GFX_PI  3.14159265358979323846
#define GFX_TAU 6.28318530718

static const float gfxMat4Identity[] = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
};

typedef float GfxMat4[16];
#define GFX_MAT4_AT(m, i, j) m[4*(j)+(i)]

void  gfxMat4Mul(GfxMat4 out, const GfxMat4 a, const GfxMat4 b);


#endif
