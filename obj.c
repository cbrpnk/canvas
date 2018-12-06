#include <GL/glew.h>
#include <stdlib.h>
#include <string.h>

#include "obj.h"
#include "canvas.h"

void canvasObjInit(CanvasObj *obj, Canvas *c, unsigned int nVertices)
{
    obj->vertices = calloc(1, nVertices * 3 * sizeof(float));
    obj->nVertices = nVertices;
    memcpy(obj->transform, c->state->transform, sizeof(obj->transform));
    memcpy(obj->fillColor, c->state->fillColor, sizeof(obj->fillColor));
    obj->shader = c->shaderProgram;
    obj->primitives = GL_TRIANGLES;
    glGenBuffers(1, (GLuint*) &obj->vertexBuffer);
}

void canvasObjCleanup(CanvasObj *obj)
{
    free(obj->vertices);
}

void canvasObjUpdateBuffers(CanvasObj *obj)
{
    // Vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, (GLuint) obj->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 obj->nVertices * 3 * sizeof(float),
                 obj->vertices, GL_STATIC_DRAW);
}

void canvasObjSetVertex(CanvasObj *obj, unsigned int v, float x, float y)
{
    obj->vertices[v*3]   = x;
    obj->vertices[v*3+1] = y;
}
