#include <GL/glew.h>
#include <stdlib.h>
#include <string.h>

#include "obj.h"
#include "gfx.h"

void gfxObjInit(GfxObj *obj, GfxObj *parent)
{
    // Pointer to an array of vertex components x,y,z,x,y,z,...
    obj->parent = parent;
    obj->vertices = NULL;
    obj->vertCount = 0;
    obj->vertCapacity = 0;
    memcpy(obj->transform, gfxMat4Identity, sizeof(gfxMat4Identity));
    obj->fillColor[0] = 1.;
    obj->fillColor[1] = 1.;
    obj->fillColor[2] = 1.;
    obj->fillColor[3] = 1.;
    obj->hasTexture = 0;
    obj->shader = NULL;
    
    glGenBuffers(1, (GLuint*) &obj->vertexBuffer);
    obj->texture = 0;
    
    // Sub objects
    obj->subObjs = NULL;
    obj->subObjCount = 0;
}

void gfxObjCleanup(GfxObj *obj)
{
    free(obj->vertices);
    for(int i=0; i<obj->subObjCount; ++i) {
        gfxObjCleanup(&(obj->subObjs[i]));
    }
}

void gfxSetTexture(GfxObj *obj, unsigned int *texture)
{
    obj->texture = *texture;
    obj->hasTexture = 1;
}

void gfxSetShader(GfxObj *obj, GfxShader *shader)
{
    obj->shader = shader;
}

GfxObj *gfxObjNew(GfxObj *obj)
{
    obj->subObjs = realloc(obj->subObjs, (obj->subObjCount+1) * sizeof(GfxObj));
    GfxObj *newObj = &(obj->subObjs[obj->subObjCount]);
    gfxObjInit(newObj, obj);
    obj->subObjCount++;
    return newObj;
}

void gfxRenderWork(GfxObj *obj)
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, obj->vertexBuffer);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    
    // Shader TODO Make it so that every object has at least a default shader
    glUseProgram(obj->shader->glId);
    
    // Bind transform
    glUniformMatrix4fv(glGetUniformLocation(obj->shader->glId, "transform"),
                       1,
                       GL_FALSE,
                       (const GLfloat *) obj->transform);
    
    // FillColor
    glUniform3f(glGetUniformLocation(obj->shader->glId, "fillColor"),
                obj->fillColor[0],
                obj->fillColor[1],
                obj->fillColor[2]);
    
    if(obj->hasTexture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, obj->texture);
        glUniform1i(glGetUniformLocation(obj->shader->glId, "tex"), 0);
    }
    
    // Draw
    glDrawArrays(GL_TRIANGLES, 0, obj->vertCount * 3);
    
    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void gfxRender(GfxObj *obj)
{
    gfxRenderWork(obj);
    for(int i=0; i<obj->subObjCount; ++i) {
        gfxRender(&obj->subObjs[i]);
    }
}

/////////////// Transform ///////////////////

void gfxRotate(GfxObj *obj, float angle)
{
    GfxMat4 tmp;
    memcpy(tmp, gfxMat4Identity, sizeof(tmp));
    
    GFX_MAT4_AT(tmp, 0, 0) = cos(angle);
    GFX_MAT4_AT(tmp, 0, 1) = sin(angle);
    GFX_MAT4_AT(tmp, 1, 0) = -sin(angle);
    GFX_MAT4_AT(tmp, 1, 1) = cos(angle);
    
    gfxMat4Mul(obj->transform, tmp);
}

void gfxScale(GfxObj *obj, float x, float y)
{
    GfxMat4 tmp;
    memcpy(tmp, gfxMat4Identity, sizeof(tmp));
    
    GFX_MAT4_AT(tmp, 0, 0) = x;
    GFX_MAT4_AT(tmp, 1, 1) = y;
    
   gfxMat4Mul(obj->transform, tmp);
}

void gfxTranslate(GfxObj *obj, float x, float y)
{
    GfxMat4 tmp;
    memcpy(tmp, gfxMat4Identity, sizeof(tmp));
    
    GFX_MAT4_AT(tmp, 3, 0) = x;
    GFX_MAT4_AT(tmp, 3, 1) = y;
    
    gfxMat4Mul(obj->transform, tmp);
}


///////////// Used by the drawing funcntions ////////////////

void gfxObjRequire(GfxObj *obj, unsigned int nTriangles)
{
    unsigned int free = (obj->vertCapacity - obj->vertCount);
    unsigned int needed = nTriangles*3;
    if(needed > free) {
        obj->vertCapacity += needed - free;
        obj->vertices = realloc(obj->vertices, obj->vertCapacity * 3 * sizeof(float));
    }
}

void gfxObjUpdateBuffers(GfxObj *obj)
{
    // Vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, (GLuint) obj->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 obj->vertCount * 3 * sizeof(float),
                 obj->vertices, GL_STATIC_DRAW);
}

void gfxObjAddTriangle(GfxObj *obj, float x1, float y1, float x2, float y2,
                          float x3, float y3)
{
    gfxObjRequire(obj, 1);
    unsigned int v = obj->vertCount * 3;
    obj->vertices[v]   = x1;
    obj->vertices[v+1] = y1;
    obj->vertices[v+2] = 0;
    obj->vertices[v+3] = x2;
    obj->vertices[v+4] = y2;
    obj->vertices[v+5] = 0;
    obj->vertices[v+6] = x3;
    obj->vertices[v+7] = y3;
    obj->vertices[v+8] = 0;
    obj->vertCount += 3;
}

void gfxObjAddTriangleStrip(GfxObj *obj, float x, float y)
{
    float *lastVert = &obj->vertices[obj->vertCount*3 - 3];
    float *beforeLastVert = lastVert-3;
    gfxObjAddTriangle(obj, *beforeLastVert, *(beforeLastVert+1),
                         *lastVert, *(lastVert+1), x, y);
}


/////////////// Drawing ///////////////////

void gfxTri(GfxObj *obj, float x1, float y1, float x2, float y2, float x3, float y3)
{
    gfxObjAddTriangle(obj, x1, y1, x2, y2, x3, y3);
    gfxObjUpdateBuffers(obj);
}

void gfxRect(GfxObj *obj, float x, float y, float w, float h)
{
    // Precomputes half dimentions
    float hw = w/2;
    float hh = h/2;
    
    // Top left corner
    float tlx = x - hw;
    float tly = y + hh;
    
    // Bottom right corner
    float brx = x + hw;
    float bry = y - hh;
    
    gfxObjAddTriangle(obj, tlx, bry, tlx, tly, brx, bry);
    gfxObjAddTriangleStrip(obj, brx, tly);
    
    gfxObjUpdateBuffers(obj);
}

void gfxNgonArc(GfxObj *obj, float x, float y, float r, float angle,
                   unsigned int segments)
{
    float angleIncrement = angle / segments;
    float currentAngle = angleIncrement;
    
    // First Tirangle
    float lastX = cos(currentAngle)*r+x;
    float lastY = sin(currentAngle)*r+y;
    gfxObjAddTriangle(obj, x, y, x+r, y, lastX, lastY);
    
    // The rest
    for(int i=1; i<segments; ++i) {
        currentAngle += angleIncrement;
        float currentX = cos(currentAngle)*r+x;
        float currentY = sin(currentAngle)*r+y;
        gfxObjAddTriangle(obj, x, y, lastX, lastY, currentX, currentY);
        lastX = currentX;
        lastY = currentY;
    }
    
    gfxObjUpdateBuffers(obj);
}

void gfxNgon(GfxObj *obj, float x, float y, float r, float n)
{
    gfxNgonArc(obj, x, y, r, GFX_TAU, n);
}

void gfxArc(GfxObj *obj, float x, float y, float r, float angle)
{
    gfxNgonArc(obj, x, y, r, angle, 32);
}

void gfxCirc(GfxObj *obj, float x, float y, float r)
{
    gfxArc(obj, x, y, r, GFX_TAU);
}

void gfxLine(GfxObj *obj, float x1, float y1, float x2, float y2)
{
    // TODO Remove hardcoded value
    float halfStroke =  3.;
    
    // Position one tip of the line at the origin
    float x = x2 - x1;
    float y = y2 - y1;
    
    // Get Perpendicular vector
    float px = y;
    float py = -x;
    
    // Make perpendicular vector have the size of the stroke width
    float len = sqrt(px*px + py*py);
    px *= halfStroke/len;
    py *= halfStroke/len;
    
    // Draw quad
    gfxObjAddTriangle(obj, px+x1, py+y1, -px+x1, -py+y1, px+x2, py+y2);
    gfxObjAddTriangleStrip(obj, -px+x2, -py+y2);
    
    gfxObjUpdateBuffers(obj);
}
