#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

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
    
    Canvas *c = canvasInit();
    canvasRadialGrad(c, .1, .4, .8, 1., 1., 0., 0., 1.);
    canvasFillCirc(c, .7, .7, .1);
    canvasFillCirc(c, .2, .2, .05);
    canvasDraw(c);
    
    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        canvasRender(c);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    canvasCleanup(c);
    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}
