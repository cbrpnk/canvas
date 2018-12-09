#define GLEW_STATIC
#include <GL/glew.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "shader.h"
#include "gfx.h"
#include "obj.h"

Gfx *gfxInit(unsigned int width, unsigned int height)
{
    Gfx *c = malloc(sizeof(Gfx));
    c->width = width;
    c->height = height;
    
    // Opengl settings
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_MULTISAMPLE);
    //glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    return c;
}

void gfxCleanup(Gfx *c)
{
    free(c);
}

void gfxSetSize(Gfx *c, unsigned int width, unsigned int height)
{
    // Transform
    //memcpy(c->state->transform, gfxMat4Identity, sizeof(gfxMat4Identity));
    //gfxTranslate(c, -1.0, -1.0);
    //gfxScale(c, 2./width, 2./height);
}
