#ifndef GFX_TEXTURE_H
#define GFX_TEXTURE_H

typedef struct GfxTexture {
    unsigned int width;
    unsigned int height;
    unsigned int glId;
} GfxTexture;

void gfxTextureInit(GfxTexture *tex, const char *path);
void gfxTextureCleanup();

#endif
