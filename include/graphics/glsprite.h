#ifndef ROGUELIKE_INCLUDED_GRAPHICS_GLSPRITE
#define ROGUELIKE_INCLUDED_GRAPHICS_GLSPRITE

#include <graphics/gltexture.h>
#include <graphics/glrenderctx.h>

namespace graphics {
    class GLSprite {
        const GLTexture &tex;
        int width, height;
        GLint data[16];
        void updatePositionData() {
            const int l = data[0], r = data[0] + width;
            const int u = data[1], b = data[1] + height;
            data[2] = r;
            data[3] = u;
            data[4] = l;
            data[5] = b;
            data[6] = r;
            data[7] = b;
        }
    public:
        GLSprite(const GLTexture &tex, int width, int height, float tx, float ty, float tw, float th);
        // assume VAO and program are already activated (set up for drawing)
        void draw() const;
        void setTexCoords(float tx, float ty, float tw, float th);

        void setPosition(int x, int y) {data[0] = x; data[1] = y; updatePositionData();};
        int getX() const {return data[0];}
        int getY() const {return data[1];}
        void setSize(int w, int h) {width = w; height = h;}
        int getWidth() const {return width;}
        int getHeight() const {return height;}
    };
}

#endif
