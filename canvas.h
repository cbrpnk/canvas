#ifndef CANVAS_H
#define CANVAS_H
    
static const unsigned int canvasQuadIndices[] = {
    0, 1, 3,
    3, 1, 2
};

static const float canvasQuadVertices[] = {
    -1.0f,  1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,
     1.0f,  1.0f, 0.0f
};

typedef enum CanvasCommand {
    CANVAS_STROKE = 0,
    CANVAS_FILL,
    CANVAS_FILL_CIRC = 10
} CanvasCommand;

static const float canvasCommandCode[] = {
    0.000f, // Unused
    0.001f, // Unused
    0.002f, // Unused
    0.003f, // Unused
    0.004f, // Unused
    0.005f, // Unused
    0.006f, // Unused
    0.007f, // Unused
    0.008f, // Unused
    0.009f, // Unused
    0.010f, // FILL_CIRC
};

typedef struct Canvas {
    float *commandBuffer;
    unsigned int nCommands;
    unsigned int bufferSize;
    unsigned int bufferCapacity;
    // Gl
    unsigned int shaderProgram;
    unsigned int indexBuffer;
    unsigned int vertexBuffer;
    unsigned int commandBufferTex;
} Canvas;

Canvas *canvasInit();
void canvasCleanup(Canvas *c);
void canvasRender(Canvas *c);

/////////////// Used by commands ////////////////////

#define BUFFER_SIZE_INCREMENT 100
void canvasExpandBuffer(Canvas *c);
void canvasAddCommand(Canvas *c, CanvasCommand cmd);
void canvasAddParam(Canvas *c, float param);


/////////////////// Commands ///////////////////////

int canvasStroke(Canvas *c, float size);
int canvasFillCirc(Canvas *c, float x, float y, float radius);


#endif
