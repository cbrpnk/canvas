#ifndef CANVAS_SHADER_H
#define CANVAS_SHADER_H

typedef struct CanvasShader {
    unsigned int glId;
} CanvasShader;

void canvasShaderInit(CanvasShader *shader, char *vertexPath, char *fragmentPath);
void canvasShaderCleanup(CanvasShader *shader);

#endif
