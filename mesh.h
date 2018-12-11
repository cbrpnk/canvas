#ifndef GFX_MESH_H
#define GFX_MESH_H

typedef struct GfxMesh {
    float        *vertices;
    unsigned int vertCount;
    unsigned int vertCapacity;
    unsigned int glId;
} GfxMesh;

void gfxMeshInit(GfxMesh *msh);
void gfxMeshCleanup(GfxMesh *msh);


// Require space for at least n more triangles
void gfxMeshRequire(GfxMesh *msh, unsigned int nTriangles);
// Once the drawing is done update the opengl buffer
void gfxMeshUpdateBuffers(GfxMesh *msh);
// Add triangle
void gfxMeshAddTriangle(GfxMesh *msh, float x1, float y1, float x2, float y2,
                          float x3, float y3);
// Add new triangle using the last two points and the new one
void gfxMeshAddTriangleStrip(GfxMesh *msh, float x, float y);

#endif
