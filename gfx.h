#ifndef GFX_H
#define GFX_H

#include "math.h"
#include "shader.h"
#include "obj.h"

typedef struct Gfx { 
    unsigned int width;
    unsigned int height;
} Gfx;


/////////////// Top Level Api //////////////

Gfx *gfxInit(unsigned int width, unsigned int height);
void gfxCleanup(Gfx *c);
void gfxSetSize(Gfx *c, unsigned int width, unsigned int height);
#endif
