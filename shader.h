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
    char *name;
    union Value {
        float *f;
        int   *i;
    } value;
} GfxUniform;

typedef struct GfxShader {
    unsigned int glId;
    GfxUniform *uniforms;
    unsigned int uniformCount;
} GfxShader;

void gfxShaderInit(GfxShader *shader, const char *vertexPath, const char *fragmentPath);
void gfxShaderCleanup(GfxShader *shader);

void gfxShaderAddUniform1f(GfxShader *sh, const char *name, float val);

#endif
