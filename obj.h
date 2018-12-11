#ifndef GFX_OBJ_H
#define GFX_OBJ_H

#include "math.h"
#include "mesh.h"

typedef struct GfxObj GfxObj;
typedef struct GfxShader GfxShader;
typedef struct GfxTexture GfxTexture;

typedef struct GfxObj {
    GfxMat4 transform;
    float   fillColor[3];
   
    // 
    GfxMesh    *mesh;
    GfxShader  *shader;
    GfxTexture *texture;
    
    // Obj position in the objects tree
    GfxObj       *parent;
    GfxObj       **children;
    unsigned int childrenCount;
} GfxObj;


void gfxObjInit(GfxObj *obj, GfxObj *parent, GfxMesh *mesh, GfxShader *shader);
void gfxObjCleanup(GfxObj *obj);

void gfxSetMesh(GfxObj *obj, GfxMesh *msh);
void gfxSetShader(GfxObj *obj, GfxShader *sh);
void gfxSetTexture(GfxObj *obj, GfxTexture *tex);

void gfxAddChild(GfxObj *obj, GfxObj *child);
void gfxObjRender(GfxObj *obj, GfxMat4 parentTransform);


/////////////// Transforms ///////////////////

void gfxRotate(GfxObj *obj, float angle);
void gfxScale(GfxObj *obj, float x, float y);
void gfxTranslate(GfxObj *obj, float x, float y);


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
