#include <GL/glew.h>
#include <stdlib.h>
#include "mesh.h"

void gfxMeshInit(GfxMesh *msh)
{
    msh->vertices = NULL;
    msh->vertCount = 0;
    msh->vertCapacity = 0;
    glGenBuffers(1, (GLuint*) &msh->glId);
}

void gfxMeshCleanup(GfxMesh *msh)
{
    glDeleteBuffers(1, &msh->glId);
    free(msh->vertices);
}

///////////// Used by the drawing funcntions ////////////////

void gfxMeshRequire(GfxMesh *msh, unsigned int nTriangles)
{
    unsigned int free = (msh->vertCapacity - msh->vertCount);
    unsigned int needed = nTriangles*3;
    if(needed > free) {
        msh->vertCapacity += needed - free;
        msh->vertices = realloc(msh->vertices, msh->vertCapacity * 3 * sizeof(float));
    }
}

void gfxMeshUpdateBuffers(GfxMesh *msh)
{
    // Vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, (GLuint) msh->glId);
    glBufferData(GL_ARRAY_BUFFER,
                 msh->vertCount * 3 * sizeof(float),
                 msh->vertices, GL_STATIC_DRAW);
}

void gfxMeshAddTriangle(GfxMesh *msh, float x1, float y1, float x2, float y2,
                          float x3, float y3)
{
    gfxMeshRequire(msh, 1);
    unsigned int v = msh->vertCount * 3;
    msh->vertices[v]   = x1;
    msh->vertices[v+1] = y1;
    msh->vertices[v+2] = 0;
    msh->vertices[v+3] = x2;
    msh->vertices[v+4] = y2;
    msh->vertices[v+5] = 0;
    msh->vertices[v+6] = x3;
    msh->vertices[v+7] = y3;
    msh->vertices[v+8] = 0;
    msh->vertCount += 3;
}

void gfxMeshAddTriangleStrip(GfxMesh *msh, float x, float y)
{
    float *lastVert = &msh->vertices[msh->vertCount*3 - 3];
    float *beforeLastVert = lastVert-3;
    gfxMeshAddTriangle(msh, *beforeLastVert, *(beforeLastVert+1),
                         *lastVert, *(lastVert+1), x, y);
}
