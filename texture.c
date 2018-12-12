#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "GL/glew.h"

#include "texture.h"

void gfxTextureInit(GfxTexture *tex, const char *path)
{
    int width, height, n;
    unsigned char *texData = stbi_load(path, &width, &height, &n, 0);
    
    // Create opengl buffer
    glGenTextures(1, &tex->glId);
    glBindTexture(GL_TEXTURE_2D, tex->glId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
    texData);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    stbi_image_free(texData);
}

void gfxTextureCleanup(GfxTexture *tex)
{
    glDeleteTextures(1, &tex->glId);
}
