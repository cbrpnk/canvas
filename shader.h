#ifndef GFX_SHADER_H
#define GFX_SHADER_H

typedef enum GfxUniformType {
    GFX_UNIFORM_1F,
    GFX_UNIFORM_2F,
    GFX_UNIFORM_3F,
    GFX_UNIFORM_4F,
} GfxUniformType;

typedef struct GfxUniform {
    GfxUniformType type;
    union Data {
        float *f;
        int   *i;
    } data;
} GfxUniform;

typedef struct GfxShader {
    unsigned int glId;
} GfxShader;

void gfxShaderInit(GfxShader *shader, const char *vertexPath, const char *fragmentPath);
void gfxShaderCleanup(GfxShader *shader);

#endif
