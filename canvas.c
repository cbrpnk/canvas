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
    glDrawElements(GL_TRIANGLES, sizeof(canvasQuadVertices), GL_UNSIGNED_INT, 0);
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
    // TODO Encode command into float
    c->commandBuffer[c->bufferSize++] = cmd;
}

void canvasAddParam(Canvas *c, float param)
{
    if(c->bufferSize == c->bufferCapacity) canvasExpandBuffer(c);
    c->commandBuffer[c->bufferSize++] = param;
}

/////////////////// Commands ///////////////////////

int canvasStroke(Canvas *c, float size)
{
    canvasAddCommand(c, CANVAS_STROKE);
    canvasAddParam(c, size);
    return 0;
}
