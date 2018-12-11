#include <GL/glew.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "shader.h"

void readFile(const char *path, char **outFile)
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

void gfxShaderInit(GfxShader *shader, const char *vertexPath, const char *fragmentPath)
{
    // Compile vertex shader
    char *vertexCode = NULL;
    readFile(vertexPath, &vertexCode);
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
    readFile(fragmentPath, &fragmentCode);
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
    shader->glId = glCreateProgram();
    glAttachShader(shader->glId, vertexShader);
    glAttachShader(shader->glId, fragmentShader);
    glLinkProgram(shader->glId);
    glGetProgramiv(shader->glId, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shader->glId, sizeof(infoLog), NULL, infoLog);
        printf("Link error: %s\n", infoLog);
    }
    
    // Cleanup
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    free(vertexCode);
    free(fragmentCode);
}

void gfxShaderCleanup(GfxShader *shader)
{
    glDeleteProgram(shader->glId);
}
