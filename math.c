#include <string.h>

#include "math.h"

void gfxMat4SetIdentity(GfxMat4 *m)
{
    memcpy(m, gfxMat4Identity, sizeof(gfxMat4Identity));
}

void  gfxMat4Mul(GfxMat4 out, const GfxMat4 a, const GfxMat4 b)
{
    GfxMat4 tmp;
    memset(tmp, 0, sizeof(tmp));
    
    for(int i=0; i<4; ++i) {
        for(int j=0; j<4; ++j) {
            for(int k=0; k<4; ++k) {
                GFX_MAT4_AT(tmp, i, j) += GFX_MAT4_AT(a, k, j) *
                                             GFX_MAT4_AT(b, i, k);
                //GFX_MAT4_AT(tmp, i, j) += GFX_MAT4_AT(b, i, k) *
                //                             GFX_MAT4_AT(a, k, j);
            }
        }
    }
    
    memcpy(out, tmp, sizeof(tmp));
}
