#define GLEW_STATIC
#include <GL/glew.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "gfx.h"
#include "obj.h"

Gfx *gfxInit(unsigned int width, unsigned int height)
{
    Gfx *g = malloc(sizeof(Gfx));
    
    // Meshes
    g->meshes = NULL;
    g->meshCount = 0;
    
    // Shaders
    g->shaders = NULL;
    g->shaderCount = 0;
    g->defaultShader = gfxNewShader(g, "res/shader.vert", "res/shader.frag");
    
    // Textures
    g->textures = NULL;
    g->textureCount = 0;
    
    // Objects
    g->objs = NULL;
    g->objCount = 0;
    g->root = gfxNewObj(g, NULL);
    
    // Resolution
    gfxSetResolution(g, width, height);
    
    // Opengl settings
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    return g;
}

void gfxCleanup(Gfx *g)
{
    // Meshes
    for(int i=0; i<g->meshCount; ++i) {
        gfxMeshCleanup(g->meshes[i]);
        free(g->meshes[i]);
    }
    free(g->meshes);
    
    // Objects
    for(int i=0; i<g->objCount; ++i) {
        gfxObjCleanup(g->objs[i]);
        free(g->objs[i]);
    }
    free(g->objs);
    
    // Shaders
    for(int i=0; i<g->shaderCount; ++i) {
        gfxShaderCleanup(g->shaders[i]);
        free(g->shaders[i]);
    }
    free(g->shaders);
    
    // Textures
    for(int i=0; i<g->textureCount; ++i) {
        gfxTextureCleanup(g->textures[i]);
        free(g->textures[i]);
    }
    free(g->textures);
    
    free(g);
}

void gfxSetResolution(Gfx *g, unsigned int width, unsigned int height)
{
    g->width = width;
    g->height = height;
    // Transform
    memcpy(g->root->transform, gfxMat4Identity, sizeof(gfxMat4Identity));
    gfxTranslate(g->root, -1., -1.);
    gfxScale(g->root, 2./width, 2./height);
}

void gfxRender(Gfx *g)
{
    gfxObjRender(g->root, NULL);
}

GfxObj *gfxGetRoot(Gfx *g)
{
    return g->root;
}

GfxObj *gfxNewObj(Gfx *g, GfxObj *parent)
{
    g->objs = realloc(g->objs, (g->objCount+1) * sizeof(GfxObj*));
    GfxObj *obj = malloc(sizeof(GfxObj));
    g->objs[g->objCount++] = obj;
    GfxMesh *msh = gfxNewMesh(g);
    
    gfxObjInit(obj, parent, msh, g->defaultShader);
    if(parent) {
        gfxAddChild(parent, obj);
    }
    
    return obj;
}

GfxMesh *gfxNewMesh(Gfx *g)
{
    g->meshes = realloc(g->meshes, (g->meshCount+1) * sizeof(GfxMesh*));
    GfxMesh *msh = malloc(sizeof(GfxMesh));
    g->meshes[g->meshCount++] = msh;
    
    gfxMeshInit(msh);
    return msh;
}

GfxShader *gfxNewShader(Gfx *g, const char *vertexPath, const char *fragmentPath)
{
    g->shaders = realloc(g->shaders, (g->shaderCount+1) * sizeof(GfxShader*));
    GfxShader *sh = malloc(sizeof(GfxShader));
    g->shaders[g->shaderCount++] = sh;
    
    gfxShaderInit(sh, vertexPath, fragmentPath);
    return sh;
}

GfxTexture *gfxNewTexture(Gfx *g, const char *path)
{
    g->textures = realloc(g->textures, (g->textureCount+1) * sizeof(GfxTexture*));
    GfxTexture *tex = malloc(sizeof(GfxTexture));
    g->textures[g->textureCount++] = tex;
    
    gfxTextureInit(tex, path);
    return tex;
}
