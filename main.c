#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "canvas.h"

static void keyCallback(GLFWwindow *window, int key, int scancode,
                        int action, int mods)
{
    if(action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main(int argc, char **argv)
{
    GLFWwindow *window;
    if(!glfwInit()) return -1;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    
    window = glfwCreateWindow(640, 480, "Canvas Test", NULL, NULL);
    if(!window) {
        glfwTerminate();
        return -1;
    }
    
    glfwSetKeyCallback(window, keyCallback);
    
    glfwMakeContextCurrent(window);
    
    glewInit();
     
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    
    Canvas *c = canvasInit(width, height);
    canvasSave(c);
    canvasTranslate(c, 200, 200);
    canvasRotate(c, CANVAS_TAU/12.);
    canvasNgon(c, 0, 0, 40, 6);
    canvasRestore(c);
    canvasRect(c, 100, 100, 20, 20);
    canvasTri(c, 0.0f, 0.0f, 32.0f, 48.0f, 64.0f, 0.0f);
    canvasCirc(c, 300, 300, 30);
    canvasStrokeWidth(c, 1);
    canvasLine(c, 20, 200, 100, 400);
    canvasLine(c, 100, 400, 200, 460);
    
    double t1, t2;
    while(!glfwWindowShouldClose(window)) {
        t1 = glfwGetTime();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        canvasRender(c);
        glfwSwapBuffers(window);
        glfwPollEvents();
        t2 = glfwGetTime();
        //printf("%f ", 1.0f/(t2-t1));
    }
    
    canvasCleanup(c);
    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}
