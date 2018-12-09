#define GLEW_STATIC
#include <GL/glew.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "shader.h"
#include "canvas.h"
#include "obj.h"

Canvas *canvasInit(unsigned int width, unsigned int height)
{
    Canvas *c = malloc(sizeof(Canvas));
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

void canvasCleanup(Canvas *c)
{
    free(c);
}

void canvasSetSize(Canvas *c, unsigned int width, unsigned int height)
{
    // Transform
    //memcpy(c->state->transform, canvasMat4Identity, sizeof(canvasMat4Identity));
    //canvasTranslate(c, -1.0, -1.0);
    //canvasScale(c, 2./width, 2./height);
}
