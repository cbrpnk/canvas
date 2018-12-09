#ifndef CANVAS_OBJ_H
#define CANVAS_OBJ_H

#include "math.h"

typedef struct CanvasObj CanvasObj;
typedef struct CanvasShader CanvasShader;

typedef struct CanvasObj {
    CanvasObj    *parent;
    float        *vertices;
    unsigned int vertCount;
    unsigned int vertCapacity;
    CanvasMat4   transform;
    float        fillColor[3];
    unsigned int hasTexture;
    CanvasShader *shader;
    
    // Gl stuff
    unsigned int vertexBuffer;
    unsigned int texture;
    
    // These are the objects we own
    CanvasObj    *subObjs;
    unsigned int subObjCount;
} CanvasObj;

void canvasObjInit(CanvasObj *obj, CanvasObj *parent);
void canvasObjCleanup(CanvasObj *obj);
void canvasSetTexture(CanvasObj *obj, unsigned int *texture);
void canvasSetShader(CanvasObj *obj, CanvasShader *shader);
CanvasObj *canvasObjNew(CanvasObj *obj);
void canvasRender(CanvasObj *obj);


/////////////// Transform ///////////////////

void canvasRotate(CanvasObj *obj, float angle);
void canvasScale(CanvasObj *obj, float x, float y);
void canvasTranslate(CanvasObj *obj, float x, float y);


///////////// Used by the drawing funcntions ////////////////

// Require space for at least n more triangles
void canvasObjRequire(CanvasObj *obj, unsigned int nTriangles);
// Once the drawing is done update the opengl buffer
void canvasObjUpdateBuffers(CanvasObj *obj);
// Add triangle
void canvasObjAddTriangle(CanvasObj *obj, float x1, float y1, float x2, float y2,
                          float x3, float y3);
// Add new triangle using the last two points and the new one
void canvasObjAddTriangleStrip(CanvasObj *obj, float x, float y);


/////////////// Drawing ///////////////////

void canvasTri(CanvasObj *obj, float x1, float y1, float x2, float y2, float x3, float y3);
void canvasRect(CanvasObj *obj, float x, float y, float w, float h);
void canvasNgonArc(CanvasObj *obj, float x, float y, float r, float angle,
                   unsigned int segments);
void canvasNgon(CanvasObj *obj, float x, float y, float r, float n);
void canvasArc(CanvasObj *obj, float x, float y, float r, float angle);
void canvasCirc(CanvasObj *obj, float x, float y, float r);
void canvasLine(CanvasObj *obj, float x1, float y1, float x2, float y2);


#endif
