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
    CANVAS_DRAW        = 0,
    CANVAS_ROTATE      = 1,
    CANVAS_SCALE       = 2,
    CANVAS_TRANSLATE   = 3,
    CANVAS_LINEAR_GRAD = 4,
    CANVAS_RADIAL_GRAD = 5,
    CANVAS_FILL        = 6,
    CANVAS_STROKE      = 7,
    CANVAS_FILL_CIRC   = 8,
} CanvasCommand;

static const float canvasCommandCode[] = {
    0.000f, // DRAW
    0.001f, // ROTATE
    0.002f, // SCALE
    0.003f, // TRANSLATE
    0.004f, // LINEAR_GRAD
    0.005f, // RADIAL_GRAD
    0.006f, // FILL
    0.007f, // STROKE
    0.008f, // FILL_CRIC
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

int canvasDraw(Canvas *c);
int canvasRotate(Canvas *c, float angle);
int canvasScale(Canvas *c, float factor);
int canvasTranslate(Canvas *c, float x, float y);
int canvasLinearGrad(Canvas *c, float r1, float g1, float b1, float a1,
                                float r2, float g2, float b2, float a2);
int canvasRadialGrad(Canvas *c, float r1, float g1, float b1, float a1,
                                float r2, float g2, float b2, float a2);
int canvasFill(Canvas *c, float r, float g, float b, float a);
int canvasStroke(Canvas *c, float size);
int canvasFillCirc(Canvas *c, float x, float y, float radius);


#endif
