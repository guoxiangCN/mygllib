#pragma once

#include "mygllib_types.h"

/* BlendingFactorDest */
#define GL_ZERO 0
#define GL_ONE 1
#define GL_SRC_COLOR 0x0300
#define GL_ONE_MINUS_SRC_COLOR 0x0301
#define GL_SRC_ALPHA 0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_DST_ALPHA 0x0304
#define GL_ONE_MINUS_DST_ALPHA 0x0305

namespace mygl {

struct GlBlendContext {
    GLenum blend_src;
    GLenum blend_dest;

    GlBlendContext() {
        blend_src = GL_ONE;
        blend_dest = GL_ZERO;
    }
};

}  // namespace mygl