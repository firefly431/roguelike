#ifndef ROGUELIKE_INCLUDED_GRAPHICS_GLSPRITE
#define ROGUELIKE_INCLUDED_GRAPHICS_GLSPRITE

#include "gltexture.h"

namespace graphics {
    class GLSprite {
        const GLTexture &tex;
        float tx, ty, tw, th;
    public:
        GLSprite(const Texture &tex, float tx, ty, tw, th);
        ~GLSprite();
        void drawAt(int x, int y);
    };
}

#endif
