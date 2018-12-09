#ifndef CANVAS_H
#define CANVAS_H

#include "math.h"
#include "shader.h"
#include "obj.h"

typedef struct Canvas { 
    unsigned int width;
    unsigned int height;
} Canvas;


/////////////// Top Level Api //////////////

Canvas *canvasInit(unsigned int width, unsigned int height);
void canvasCleanup(Canvas *c);
void canvasSetSize(Canvas *c, unsigned int width, unsigned int height);
#endif
