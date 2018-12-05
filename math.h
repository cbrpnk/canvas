#ifndef CANVAS_MATH_H
#define CANVAS_MATH_H

#include <math.h>

#define CANVAS_PI  3.14159265358979323846
#define CANVAS_TAU 6.28318530718

static const float canvasMat4Identity[] = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
};

typedef float CanvasMat4[16];
#define CANVAS_MAT4_AT(m, i, j) m[4*(j)+(i)]

void  canvasMat4Mul(CanvasMat4 a, const CanvasMat4 b);


#endif
