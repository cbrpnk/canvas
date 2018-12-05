#ifndef CANVAS_H
#define CANVAS_H

#include "math.h"

typedef struct CanvasObj CanvasObj;

typedef struct CanvasState {
    CanvasMat4 transform;
    float      strokeWidth;
} CanvasState;

typedef struct Canvas { 
    unsigned int width;
    unsigned int height;
    CanvasObj*   objs;
    unsigned int objSize;
    unsigned int shaderProgram;
    
    // State
    CanvasState *stateStack;
    unsigned int stateStackSize;
    unsigned int stateStackCapacity;
    // Points to the state at the top of the stack
    CanvasState *state;
} Canvas;


/////////////// Top Level Api //////////////

Canvas *canvasInit(unsigned int width, unsigned int height);
void canvasCleanup(Canvas *c);
void canvasRender(Canvas *c);


////////////////// State ////////////////////

void canvasSave(Canvas *c);
void canvasRestore(Canvas *c);


////////////////// Style ////////////////////

void canvasStrokeWidth(Canvas *c, float sw);


/////////////// Transform ///////////////////

void canvasRotate(Canvas *c, float angle);
void canvasScale(Canvas *c, float x, float y);
void canvasTranslate(Canvas *c, float x, float y);

        
/////////////// Drawing ///////////////////

void canvasTri(Canvas *c, float x1, float y1, float x2, float y2, float x3, float y3);
void canvasRect(Canvas *c, float x, float y, float w, float h);
void canvasNgonArc(Canvas *c, float x, float y, float r, float angle,
                   unsigned int segments);
void canvasNgon(Canvas *c, float x, float y, float r, float n);
void canvasArc(Canvas *c, float x, float y, float r, float angle);
void canvasCirc(Canvas *c, float x, float y, float r);
void canvasLine(Canvas *c, float x1, float y1, float x2, float y2);

#endif
