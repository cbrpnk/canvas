#ifndef GFX_SHADER_H
#define GFX_SHADER_H

typedef struct GfxShader {
    unsigned int glId;
} GfxShader;

void gfxShaderInit(GfxShader *shader, const char *vertexPath, const char *fragmentPath);
void gfxShaderCleanup(GfxShader *shader);

#endif
