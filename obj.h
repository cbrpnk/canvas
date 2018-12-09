#ifndef GFX_OBJ_H
#define GFX_OBJ_H

#include "math.h"

typedef struct GfxObj GfxObj;
typedef struct GfxShader GfxShader;

typedef struct GfxObj {
    GfxObj    *parent;
    float        *vertices;
    unsigned int vertCount;
    unsigned int vertCapacity;
    GfxMat4   transform;
    float        fillColor[3];
    unsigned int hasTexture;
    GfxShader *shader;
    
    // Gl stuff
    unsigned int vertexBuffer;
    unsigned int texture;
    
    // These are the objects we own
    GfxObj    *subObjs;
    unsigned int subObjCount;
} GfxObj;

void gfxObjInit(GfxObj *obj, GfxObj *parent);
void gfxObjCleanup(GfxObj *obj);
void gfxSetTexture(GfxObj *obj, unsigned int *texture);
void gfxSetShader(GfxObj *obj, GfxShader *shader);
GfxObj *gfxObjNew(GfxObj *obj);
void gfxRender(GfxObj *obj);


/////////////// Transform ///////////////////

void gfxRotate(GfxObj *obj, float angle);
void gfxScale(GfxObj *obj, float x, float y);
void gfxTranslate(GfxObj *obj, float x, float y);


///////////// Used by the drawing funcntions ////////////////

// Require space for at least n more triangles
void gfxObjRequire(GfxObj *obj, unsigned int nTriangles);
// Once the drawing is done update the opengl buffer
void gfxObjUpdateBuffers(GfxObj *obj);
// Add triangle
void gfxObjAddTriangle(GfxObj *obj, float x1, float y1, float x2, float y2,
                          float x3, float y3);
// Add new triangle using the last two points and the new one
void gfxObjAddTriangleStrip(GfxObj *obj, float x, float y);


/////////////// Drawing ///////////////////

void gfxTri(GfxObj *obj, float x1, float y1, float x2, float y2, float x3, float y3);
void gfxRect(GfxObj *obj, float x, float y, float w, float h);
void gfxNgonArc(GfxObj *obj, float x, float y, float r, float angle,
                   unsigned int segments);
void gfxNgon(GfxObj *obj, float x, float y, float r, float n);
void gfxArc(GfxObj *obj, float x, float y, float r, float angle);
void gfxCirc(GfxObj *obj, float x, float y, float r);
void gfxLine(GfxObj *obj, float x1, float y1, float x2, float y2);


#endif
