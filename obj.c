#include <GL/glew.h>
#include <stdlib.h>
#include <string.h>

#include "obj.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"

void gfxObjInit(GfxObj *obj, GfxObj *parent, GfxMesh *msh,  GfxShader *sh)
{
    // Pointer to an array of vertex components x,y,z,x,y,z,...
    memcpy(obj->transform, gfxMat4Identity, sizeof(gfxMat4Identity));
    obj->fillColor[0] = 1.;
    obj->fillColor[1] = 1.;
    obj->fillColor[2] = 1.;
    obj->fillColor[3] = 1.;
    
    // Mesh
    obj->mesh = msh;
    obj->shader = sh;
    obj->texture = NULL;
    
    // Children
    obj->parent = parent;
    obj->children = NULL;
    obj->childrenCount = 0;
}

void gfxObjCleanup(GfxObj *obj)
{
    free(obj->children);
}

void gfxSetMesh(GfxObj *obj, GfxMesh *msh)
{
    obj->mesh = msh;
}

void gfxSetShader(GfxObj *obj, GfxShader *sh)
{
    obj->shader = sh;
}

void gfxSetTexture(GfxObj *obj, GfxTexture *tex)
{
    obj->texture = tex;
}

void gfxAddChild(GfxObj *obj, GfxObj *child)
{
    obj->children = realloc(obj->children, (obj->childrenCount+1) * sizeof(GfxObj*));
    obj->children[obj->childrenCount] = child;
    obj->childrenCount++;
}

void gfxRenderWork(GfxObj *obj, GfxMat4 parentTransform)
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, obj->mesh->glId);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    
    glUseProgram(obj->shader->glId);
    
    // Bind transform
    GfxMat4 txform;
    if(parentTransform != NULL) {
        gfxMat4Mul(txform, parentTransform, obj->transform);
    } else {
        memcpy(txform, obj->transform, sizeof(obj->transform));
    }
    glUniformMatrix4fv(glGetUniformLocation(obj->shader->glId, "transform"),
                       1,
                       GL_FALSE,
                       (const GLfloat *) txform);
    
    // FillColor
    glUniform3f(glGetUniformLocation(obj->shader->glId, "fillColor"),
                obj->fillColor[0],
                obj->fillColor[1],
                obj->fillColor[2]);
    
    // TODO Have a uniform system in the shader module
    if(obj->texture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, obj->texture->glId);
        glUniform1i(glGetUniformLocation(obj->shader->glId, "hasTexture"), 1);
        glUniform1i(glGetUniformLocation(obj->shader->glId, "tex"), 0);
    } else {
        glUniform1i(glGetUniformLocation(obj->shader->glId, "hasTexture"), 0);
    }
    
    // Draw
    glDrawArrays(GL_TRIANGLES, 0, obj->mesh->vertCount * 3);
    
    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void gfxObjRender(GfxObj *obj, GfxMat4 parentTransform)
{
    gfxRenderWork(obj, parentTransform);
    
    GfxMat4 newTransform;
    if(parentTransform != NULL) {
        gfxMat4Mul(newTransform, parentTransform, obj->transform);
    } else {
        memcpy(newTransform, obj->transform, sizeof(obj->transform));
    }
    for(int i=0; i<obj->childrenCount; ++i) {
        gfxObjRender(obj->children[i], newTransform);
    }
}

/////////////// Transforms ///////////////////

void gfxRotate(GfxObj *obj, float angle)
{
    GfxMat4 tmp;
    memcpy(tmp, gfxMat4Identity, sizeof(tmp));
    
    GFX_MAT4_AT(tmp, 0, 0) = cos(angle);
    GFX_MAT4_AT(tmp, 0, 1) = sin(angle);
    GFX_MAT4_AT(tmp, 1, 0) = -sin(angle);
    GFX_MAT4_AT(tmp, 1, 1) = cos(angle);
    
    gfxMat4Mul(obj->transform, obj->transform, tmp);
}

void gfxScale(GfxObj *obj, float x, float y)
{
    GfxMat4 tmp;
    memcpy(tmp, gfxMat4Identity, sizeof(tmp));
    
    GFX_MAT4_AT(tmp, 0, 0) = x;
    GFX_MAT4_AT(tmp, 1, 1) = y;
    
   gfxMat4Mul(obj->transform, obj->transform, tmp);
}

void gfxTranslate(GfxObj *obj, float x, float y)
{
    GfxMat4 tmp;
    memcpy(tmp, gfxMat4Identity, sizeof(tmp));
    
    GFX_MAT4_AT(tmp, 3, 0) = x;
    GFX_MAT4_AT(tmp, 3, 1) = y;
    
    gfxMat4Mul(obj->transform, obj->transform, tmp);
}


/////////////// Drawing ///////////////////

void gfxTri(GfxObj *obj, float x1, float y1, float x2, float y2, float x3, float y3)
{
    gfxMeshAddTriangle(obj->mesh, x1, y1, x2, y2, x3, y3);
    gfxMeshUpdateBuffers(obj->mesh);
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
    
    gfxMeshAddTriangle(obj->mesh, tlx, bry, tlx, tly, brx, bry);
    gfxMeshAddTriangleStrip(obj->mesh, brx, tly);
    
    gfxMeshUpdateBuffers(obj->mesh);
}

void gfxNgonArc(GfxObj *obj, float x, float y, float r, float angle,
                   unsigned int segments)
{
    float angleIncrement = angle / segments;
    float currentAngle = angleIncrement;
    
    // First Tirangle
    float lastX = cos(currentAngle)*r+x;
    float lastY = sin(currentAngle)*r+y;
    gfxMeshAddTriangle(obj->mesh, x, y, x+r, y, lastX, lastY);
    
    // The rest
    for(int i=1; i<segments; ++i) {
        currentAngle += angleIncrement;
        float currentX = cos(currentAngle)*r+x;
        float currentY = sin(currentAngle)*r+y;
        gfxMeshAddTriangle(obj->mesh, x, y, lastX, lastY, currentX, currentY);
        lastX = currentX;
        lastY = currentY;
    }
    
    gfxMeshUpdateBuffers(obj->mesh);
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
    gfxMeshAddTriangle(obj->mesh, px+x1, py+y1, -px+x1, -py+y1, px+x2, py+y2);
    gfxMeshAddTriangleStrip(obj->mesh, -px+x2, -py+y2);
    
    gfxMeshUpdateBuffers(obj->mesh);
}
