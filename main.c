#include <stdio.h>

#include "canvas.h"

int main(int argc, char **argv)
{
    Canvas *c = canvasInit();
    canvasStroke(c, 0.003);
    
    canvasRender(c);
    
    return 0;
}
