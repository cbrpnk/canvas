#include <GLFW/glfw3.h>
#include <stdio.h>

#include "canvas.h"

int main(int argc, char **argv)
{
    GLFWwindow *window;
    if(!glfwInit()) return -1;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    
    window = glfwCreateWindow(640, 480, "Canvas Test", NULL, NULL);
    if(!window) {
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    Canvas *c = canvasInit();
    canvasStroke(c, 0.003);
    
    canvasRender(c);
    
    return 0;
}
