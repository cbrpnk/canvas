#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "gfx.h"

Gfx *c;

static void keyCallback(GLFWwindow *window, int key, int scancode,
                        int action, int mods)
{
    if(action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

static void windowSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    //gfxSetSize(c, width, height);
}

int main(int argc, char **argv)
{
    GLFWwindow *window;
    if(!glfwInit()) return -1;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_SAMPLES, 4);
    
    window = glfwCreateWindow(640, 480, "Gfx Test", NULL, NULL);
    if(!window) {
        glfwTerminate();
        return -1;
    }
    
    glfwSetKeyCallback(window, keyCallback);
    
    glfwMakeContextCurrent(window);
    
    glewInit();
     
    glfwSetWindowSizeCallback(window, windowSizeCallback);
    
    // Gfx
    GfxObj master;
    gfxObjInit(&master, NULL);
    GfxShader shader;
    gfxShaderInit(&shader, "res/shader.vert", "res/shader.frag");
    gfxSetShader(&master, &shader);
    gfxTri(&master, -.5f, -.5f, 0.0f, .5f, .5, -.5);
    
    /*
    gfxObj master;
    gfxObjInit(&master, NULL);
    gfxShader shader;
    gfxShaderInit(&shader, "res/shader.vert", "res/shader.frag");
    gfxSetShader(&master, &shader);
    gfxTri(&master, -.5f, -.5f, 0.0f, .5f, .5, -.5);
    */
    
    double t1, t2;
    while(!glfwWindowShouldClose(window)) {
        t1 = glfwGetTime();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        gfxRender(&master);
        glfwSwapBuffers(window);
        glfwPollEvents();
        t2 = glfwGetTime();
        //float depth;
        //glReadPixels(380, 260, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
        //printf("%f ", depth);
        //printf("%f ", 1.0f/(t2-t1));
    }
    
    //gfxCleanup(c);
    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}
