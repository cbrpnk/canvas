#include <string.h>

#include "math.h"

void canvasMat4SetIdentity(CanvasMat4 *m)
{
    memcpy(m, canvasMat4Identity, sizeof(canvasMat4Identity));
}

void canvasMat4Mul(CanvasMat4 a, const CanvasMat4 b)
{
    CanvasMat4 tmp;
    memset(tmp, 0, sizeof(tmp));
    
    for(int i=0; i<4; ++i) {
        for(int j=0; j<4; ++j) {
            for(int k=0; k<4; ++k) {
                CANVAS_MAT4_AT(tmp, i, j) += CANVAS_MAT4_AT(a, k, j) *
                                             CANVAS_MAT4_AT(b, i, k);
            }
        }
    }
    
    memcpy(a, tmp, sizeof(tmp));
}
