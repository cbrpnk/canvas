#define GLEW_STATIC
#include <GL/glew.h>
#include <stdlib.h>
#include <stdio.h>
#include "canvas.h"

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

Canvas *canvasInit()
{
    Canvas *c = malloc(sizeof(Canvas));
    c->nCommands = 0;
    c->commandBuffer = NULL;
    c->bufferSize = 0;
    c->bufferCapacity = 0;
    canvasExpandBuffer(c);
    
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
    
    // Index Buffers
    glGenBuffers(1, (GLuint*) &c->indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (GLuint) c->indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(canvasQuadIndices) * sizeof(unsigned int),
                 canvasQuadIndices, GL_STATIC_DRAW);

    // Vertex Buffer
    glGenBuffers(1, (GLuint*) &c->vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, (GLuint) c->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(canvasQuadVertices) * sizeof(float),
                 canvasQuadVertices, GL_STATIC_DRAW);
    
    // Command Buffer Texture
    glGenTextures(1, &c->commandBufferTex);
    glBindTexture(GL_TEXTURE_1D, c->commandBufferTex);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_1D, 0);
    
    // Opengl settings
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    
    // Cleanup
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    free(vertexCode);
    free(fragmentCode);
    
    return c;
}

void canvasCleanup(Canvas *c)
{
    free(c->commandBuffer);
    free(c);
}

void canvasRender(Canvas *c)
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, c->indexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, c->vertexBuffer);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glUseProgram(c->shaderProgram);
    
    // Bind resolution
    glUniform2f(glGetUniformLocation(c->shaderProgram, "iResolution"), 640, 480);
    
    // Bind nCommands
    glUniform1i(glGetUniformLocation(c->shaderProgram, "nCommands"), c->nCommands);
    
    // Bind CmdSize
    glUniform1i(glGetUniformLocation(c->shaderProgram, "commandSize"), c->bufferSize);
    
    // Bind command buffer
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_1D, c->commandBufferTex);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA32F, c->bufferSize, 0, GL_RGBA, GL_FLOAT,
                 c->commandBuffer);
    glUniform1i(glGetUniformLocation(c->shaderProgram, "commandBuffer"), 0);
    
    glDrawElements(GL_TRIANGLES, sizeof(canvasQuadVertices), GL_UNSIGNED_INT, 0);
    
    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableClientState(GL_VERTEX_ARRAY);
}


/////////////// Used by commands ////////////////////

void canvasExpandBuffer(Canvas *c)
{
    c->commandBuffer = realloc(c->commandBuffer, sizeof(float) *
                               (c->bufferCapacity + BUFFER_SIZE_INCREMENT));
}

void canvasAddCommand(Canvas *c, CanvasCommand cmd)
{
    if(c->bufferSize == c->bufferCapacity) canvasExpandBuffer(c);
    c->nCommands++;
    c->commandBuffer[c->bufferSize++] = canvasCommandCode[cmd];
}

void canvasAddParam(Canvas *c, float param)
{
    if(c->bufferSize == c->bufferCapacity) canvasExpandBuffer(c);
    // Map [-10, 10] -> [0, 1]
    c->commandBuffer[c->bufferSize++] = (param + 10.0f) / 20.0f;
}

/////////////////// Commands ///////////////////////

int canvasDraw(Canvas *c)
{
    canvasAddCommand(c, CANVAS_DRAW);
    return 0;
}

int canvasRotate(Canvas *c, float angle)
{
    canvasAddCommand(c, CANVAS_ROTATE);
    canvasAddParam(c, angle);
    return 0;
}

int canvasScale(Canvas *c, float factor)
{
    canvasAddCommand(c, CANVAS_SCALE);
    canvasAddParam(c, factor);
    return 0;
}

int canvasTranslate(Canvas *c, float x, float y)
{
    canvasAddCommand(c, CANVAS_TRANSLATE);
    canvasAddParam(c, x);
    canvasAddParam(c, y);
    return 0;
}

int canvasLinearGrad(Canvas *c, float r1, float g1, float b1, float a1,
                                float r2, float g2, float b2, float a2)
{
    canvasAddCommand(c, CANVAS_LINEAR_GRAD);
    canvasAddParam(c, r1);
    canvasAddParam(c, g1);
    canvasAddParam(c, b1);
    canvasAddParam(c, a1);
    canvasAddParam(c, r2);
    canvasAddParam(c, g2);
    canvasAddParam(c, b2);
    canvasAddParam(c, a2);
    return 0;
}

int canvasRadialGrad(Canvas *c, float r1, float g1, float b1, float a1,
                                float r2, float g2, float b2, float a2)
{
    canvasAddCommand(c, CANVAS_RADIAL_GRAD);
    canvasAddParam(c, r1);
    canvasAddParam(c, g1);
    canvasAddParam(c, b1);
    canvasAddParam(c, a1);
    canvasAddParam(c, r2);
    canvasAddParam(c, g2);
    canvasAddParam(c, b2);
    canvasAddParam(c, a2);
    return 0;
}

int canvasFill(Canvas *c, float r, float g, float b, float a)
{
    canvasAddCommand(c, CANVAS_FILL);
    canvasAddParam(c, r);
    canvasAddParam(c, g);
    canvasAddParam(c, b);
    canvasAddParam(c, a);
    return 0;
}

int canvasStroke(Canvas *c, float size)
{
    canvasAddCommand(c, CANVAS_STROKE);
    canvasAddParam(c, size);
    return 0;
}

int canvasFillCirc(Canvas *c, float x, float y, float radius)
{
    canvasAddCommand(c, CANVAS_FILL_CIRC);
    canvasAddParam(c, x);
    canvasAddParam(c, y);
    canvasAddParam(c, radius);
    return 0;
}
