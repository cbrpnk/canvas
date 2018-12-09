#ifndef CANVAS_TEXTURE_H
#define CANVAS_TEXTURE_H

typedef struct CanvasTexture {
    unsigned int width;
    unsigned int height;
    unsigned int glId;
} CanvasTexture;

void canvasTextureInit(CanvasTexture *tex, char *path);
void canvasTextureCleanup();

#endif
