#ifndef GFX_H
#define GFX_H

#include "math.h"
#include "obj.h"
#include "obj.h"
#include "shader.h"
#include "texture.h"


// This obj saves the state of Gfx and manages memory

typedef struct Gfx { 
    // Meshes
    GfxMesh      **meshes;
    unsigned int meshCount;
    
    // Shaders
    GfxShader    **shaders;
    unsigned int shaderCount;
    GfxShader    *defaultShader;      // Every objects gets this shader at creation
    
    // Textures
    GfxTexture   **textures;
    unsigned int textureCount;
                                      // TODO Typography texture
    
    // Objects
    GfxObj       **objs;
    unsigned int objCount;
    GfxObj       *root;               // Every obj descend from root
    
    // Resolution
    unsigned int width;
    unsigned int height;
} Gfx;

Gfx *gfxInit(unsigned int width, unsigned int height);
void gfxCleanup(Gfx *g);

void gfxSetResolution(Gfx *g, unsigned int width, unsigned int height);
void gfxRender(Gfx *g);

GfxObj     *gfxGetRoot();
GfxObj     *gfxNewObj(Gfx *g, GfxObj *parent);
GfxMesh    *gfxNewMesh(Gfx *g);
GfxShader  *gfxNewShader(Gfx *g, const char *vertexPath, const char *fragmentPath);
GfxTexture *gfxNewTexture(Gfx *g, const char *path);


#endif
