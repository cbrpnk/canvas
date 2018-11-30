#include <stdlib.h>
#include <stdio.h>
#include "canvas.h"

Canvas *canvasInit()
{
    Canvas *c = malloc(sizeof(Canvas));
    c->bufferSize = 0;
    c->bufferCapacity = 0;
    canvasExpandBuffer(c);
    
    return c;
}

void canvasCleanup(Canvas *c)
{
    free(c->commandBuffer);
    free(c);
}

void canvasRender(Canvas *c)
{
    // Debug
    for(int i=0; i<c->bufferSize; ++i) {
        printf("%f ", c->commandBuffer[i]);
    }
    printf("\n");
}


/////////////// Used by commands ////////////////////

void canvasExpandBuffer(Canvas *c)
{
    c->commandBuffer = realloc(c->commandBuffer, sizeof(float) *
                               (c->bufferCapacity + BUFFER_SIZE_INCREMENT));
}

void canvasAddCommand(Canvas *c, CanvasCommand cmd)
{
    if(c->bufferSize == c->bufferCapacity) canvasExpandBuffer(c);
    // TODO Encode command into float
    c->commandBuffer[c->bufferSize++] = cmd;
}

void canvasAddParam(Canvas *c, float param)
{
    if(c->bufferSize == c->bufferCapacity) canvasExpandBuffer(c);
    c->commandBuffer[c->bufferSize++] = param;
}

/////////////////// Commands ///////////////////////

int canvasStroke(Canvas *c, float size)
{
    canvasAddCommand(c, CANVAS_STROKE);
    canvasAddParam(c, size);
    return 0;
}
