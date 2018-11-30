#ifndef CANVAS_H
#define CANVAS_H

typedef enum CanvasCommand {
    CANVAS_STROKE = 0,
    CANVAS_FILL,
} CanvasCommand;

typedef struct Canvas {
    float *commandBuffer;
    unsigned int bufferSize;
    unsigned int bufferCapacity;
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

int canvasStroke(Canvas *canvas, float size);


#endif
