#ifndef CANVAS_OBJ_H
#define CANVAS_OBJ_H

#include "math.h"

typedef struct Canvas Canvas;

typedef struct CanvasObj {
    float        *vertices;
    unsigned int nVertices;
    CanvasMat4   transform;
    float        fillColor[3];
    
    // Gl stuff
    unsigned int shader;
    unsigned int primitives;
    unsigned int vertexBuffer;
} CanvasObj;

void canvasObjInit(CanvasObj *obj, Canvas *c, unsigned int nVertices);
void canvasObjCleanup(CanvasObj *obj);
void canvasObjUpdateBuffers(CanvasObj *obj);
void canvasObjSetVertex(CanvasObj *obj, unsigned int v, float x, float y);

#endif
