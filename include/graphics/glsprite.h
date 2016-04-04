#ifndef ROGUELIKE_INCLUDED_GRAPHICS_GLSPRITE
#define ROGUELIKE_INCLUDED_GRAPHICS_GLSPRITE

#include "gltexture.h"

namespace graphics {
    class GLSprite {
        const GLTexture &tex;
        int width, height;
        GLint data[16];
    public:
        GLSprite(const GLTexture &tex, int width, int height, float tx, float ty, float tw, float th);
        // assume VAO and program are already activated (set up for drawing)
        void draw(const GLRenderContext &ctx) const;
        void setTexCoords(float tx, float ty, float tw, float th);
    };
}

#endif
