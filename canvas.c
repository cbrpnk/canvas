#define GLEW_STATIC
#include <GL/glew.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "canvas.h"
#include "obj.h"

void readShaderFile(const char *path, char **outFile)
{
    FILE *fp = fopen(path, "rb");
    if(!fp) {
        printf("Can't open file %s\n", path);
        exit(1);
    }
    
    fseek(fp, 0L, SEEK_END);
    long fileSize = ftell(fp);
    rewind(fp);
    *outFile = calloc(1, fileSize+1);
    int a=0;
    if(fread(*outFile, fileSize, 1, fp) != 1) {
        free(*outFile);
        exit(1);
    }
    fclose(fp);
}

Canvas *canvasInit(unsigned int width, unsigned int height)
{
    Canvas *c = malloc(sizeof(Canvas));
    c->width = width;
    c->height = height;
    c->objs = NULL;
    c->objSize = 0;
    
    // State
    c->stateStack = malloc(sizeof(CanvasState));
    c->stateStackSize = 1;
    c->stateStackCapacity = 1;
    c->state = c->stateStack;
    
    // Transform
    memcpy(c->state->transform, canvasMat4Identity, sizeof(canvasMat4Identity));
    canvasTranslate(c, -1.0, -1.0);
    canvasScale(c, 2./width, 2./height);
    
    // Other state attributes
    c->state->strokeWidth = 2;
    
    // Compile vertex shader
    char *vertexCode = NULL;
    readShaderFile("shader.vert", &vertexCode);
    const char * constVertexCode = vertexCode;
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &constVertexCode, NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
        printf("Vertex shader error: %s\n", infoLog);
    }
     
    // Compile fragment shader
    char *fragmentCode = NULL;
    readShaderFile("shader.frag", &fragmentCode);
    const char * constFragmentCode = fragmentCode;
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &constFragmentCode, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragmentShader, sizeof(infoLog), NULL, infoLog);
        printf("Fragment shader error: %s\n", infoLog);
    }
    
    // Link shader program
    c->shaderProgram = glCreateProgram();
    glAttachShader(c->shaderProgram, vertexShader);
    glAttachShader(c->shaderProgram, fragmentShader);
    glLinkProgram(c->shaderProgram);
    glGetProgramiv(c->shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(c->shaderProgram, sizeof(infoLog), NULL, infoLog);
        printf("Link error: %s\n", infoLog);
    }
    
    // Opengl settings
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    
    // Cleanup
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    free(vertexCode);
    free(fragmentCode);
    
    return c;
}

void canvasCleanup(Canvas *c)
{
    for(int i=0; i<c->objSize; ++i) {
        canvasObjCleanup(&c->objs[i]);
    }
    free(c->objs);
    free(c->stateStack);
    free(c);
}

CanvasObj *canvasAddObj(Canvas *c, unsigned int nVertices)
{
    c->objs = realloc(c->objs, (c->objSize+1) * sizeof(CanvasObj));
    CanvasObj *obj = &(c->objs[c->objSize]);
    canvasObjInit(obj, c, nVertices);
    c->objSize++;
    return obj;
}

void canvasRender(Canvas *c)
{
    for(int i=0; i<c->objSize; ++i) {
        CanvasObj *obj = &c->objs[i];
        
        glEnableClientState(GL_VERTEX_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, obj->vertexBuffer);
        glVertexPointer(3, GL_FLOAT, 0, 0);
        
        // Shader
        glUseProgram(obj->shader);
        
        // Bind transform
        glUniformMatrix4fv(glGetUniformLocation(c->shaderProgram, "transform"),
                           1,
                           GL_FALSE,
                           (const GLfloat *) obj->transform);
        
        // Draw
        glDrawArrays(obj->primitives, 0, obj->nVertices);
        
        // Unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
}

////////////////// State ////////////////////

void canvasSave(Canvas *c)
{
    if(c->stateStackSize == c->stateStackCapacity) {
        c->stateStackCapacity++;
        c->stateStack = realloc(c->stateStack,
                                c->stateStackCapacity * sizeof(CanvasState));
    }
    c->state = c->stateStack + c->stateStackSize;
    memcpy(c->state, c->state-1, sizeof(CanvasState));
    c->stateStackSize++;
}

void canvasRestore(Canvas *c)
{
    if(c->stateStackSize > 1) {
        c->stateStackSize--;
        c->state--;
    }
}

///////////////// Style ///////////////////

void canvasStrokeWidth(Canvas *c, float sw)
{
    c->state->strokeWidth = sw;
}

/////////////// Transform /////////////////

void canvasRotate(Canvas *c, float angle)
{
    CanvasMat4 tmp;
    memcpy(tmp, canvasMat4Identity, sizeof(tmp));
    
    CANVAS_MAT4_AT(tmp, 0, 0) = cos(angle);
    CANVAS_MAT4_AT(tmp, 0, 1) = sin(angle);
    CANVAS_MAT4_AT(tmp, 1, 0) = -sin(angle);
    CANVAS_MAT4_AT(tmp, 1, 1) = cos(angle);
    
    canvasMat4Mul(c->state->transform, tmp);
}

void canvasScale(Canvas *c, float x, float y)
{
    CanvasMat4 tmp;
    memcpy(tmp, canvasMat4Identity, sizeof(tmp));
    
    CANVAS_MAT4_AT(tmp, 0, 0) = x;
    CANVAS_MAT4_AT(tmp, 1, 1) = y;
    
    canvasMat4Mul(c->state->transform, tmp);
}

void canvasTranslate(Canvas *c, float x, float y)
{
    CanvasMat4 tmp;
    memcpy(tmp, canvasMat4Identity, sizeof(tmp));
    
    CANVAS_MAT4_AT(tmp, 3, 0) = x;
    CANVAS_MAT4_AT(tmp, 3, 1) = y;
    
    canvasMat4Mul(c->state->transform, tmp);
}

/////////////// Drawing ///////////////////

void canvasTri(Canvas *c, float x1, float y1, float x2, float y2, float x3, float y3)
{
    CanvasObj *obj = canvasAddObj(c, 3);
    
    // Vertices
    canvasObjSetVertex(obj, 0, x1, y1);
    canvasObjSetVertex(obj, 1, x2, y2);
    canvasObjSetVertex(obj, 2, x3, y3);
    
    canvasObjUpdateBuffers(obj);
}

void canvasRect(Canvas *c, float x, float y, float w, float h)
{
    CanvasObj *obj = canvasAddObj(c, 4);
    obj->primitives = GL_TRIANGLE_STRIP;
    
    // Precomputes half dimentions
    float hw = w/2;
    float hh = h/2;
    
    // Top left corner
    float tlx = x - hw;
    float tly = y + hh;
    
    // Bottom right corner
    float brx = x + hw;
    float bry = y - hh;
    
    canvasObjSetVertex(obj, 0, tlx, bry); // Bottom left
    canvasObjSetVertex(obj, 1, tlx, tly); // Top lef
    canvasObjSetVertex(obj, 2, brx, bry); // Bottom right
    canvasObjSetVertex(obj, 3, brx, tly); // Top right
    
    canvasObjUpdateBuffers(obj);
}

void canvasNgonArc(Canvas *c, float x, float y, float r, float angle,
                   unsigned int segments)
{
    CanvasObj *obj = canvasAddObj(c, segments+2);
    obj->primitives = GL_TRIANGLE_FAN;
    
    canvasObjSetVertex(obj, 0, x, y); // Center
    
    float angleIncrement = angle / segments;
    float currentAngle = 0;
    for(int i=0; i<=segments; ++i) {
        float vertX = (cos(currentAngle) * r) + x;
        float vertY = (sin(currentAngle) * r) + y;
        canvasObjSetVertex(obj, i+1, vertX, vertY);
        currentAngle += angleIncrement;
    }
    
    canvasObjUpdateBuffers(obj);
}

void canvasNgon(Canvas *c, float x, float y, float r, float n)
{
    canvasNgonArc(c, x, y, r, CANVAS_TAU, n);
}

void canvasArc(Canvas *c, float x, float y, float r, float angle)
{
    canvasNgonArc(c, x, y, r, angle, 32);
}

void canvasCirc(Canvas *c, float x, float y, float r)
{
    canvasArc(c, x, y, r, CANVAS_TAU);
}

void canvasLine(Canvas *c, float x1, float y1, float x2, float y2)
{
    CanvasObj *obj = canvasAddObj(c, 4);
    obj->primitives = GL_TRIANGLE_STRIP;
    
    // 
    float halfStroke = c->state->strokeWidth / 2;
    
    // Position one tip of the line at the origin
    float x = x2 - x1;
    float y = y2 - y1;
    
    // Get Perpendicular vector
    float px = y;
    float py = -x;
    
    // Make perpendicular vector have the size of the stroke width
    float len = sqrt(px*px + py*py);
    px *= halfStroke/len;
    py *= halfStroke/len;
    
    // Draw quad
    canvasObjSetVertex(obj, 0, px+x1, py+y1);
    canvasObjSetVertex(obj, 1, -px+x1, -py+y1);
    canvasObjSetVertex(obj, 2, px+x2, py+y2);
    canvasObjSetVertex(obj, 3, -px+x2, -py+y2);
    
    canvasObjUpdateBuffers(obj);
}
