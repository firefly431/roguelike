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
        void draw(const RenderContext &ctx) const;
        void setTexCoords(float tx, float ty, float tw, float th);

        void setPosition(int x, int y) {data[0] = x; data[1] = y};
        int getX() const {return data[0];}
        int getY() const {return data[1];}
        void setSize(int w, int h) {width = w; height = h;}
        int getWidth() const {return width;}
        int getHeight() const {return height;}
    };
}

#endif
