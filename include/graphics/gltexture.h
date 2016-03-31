#ifndef ROGUELIKE_INCLUDED_GRAPHICS_GLTEXTURE
#define ROGUELIKE_INCLUDED_GRAPHICS_GLTEXTURE

#include <GL/glew.h>

#include <lodepng.h>

#include <string>

namespace graphics {
    class GLTexture {
        GLuint id;
        static GLuint activeTexture; // used to optimize texture switching
    public:
        Texture(const std::string &fname);
        ~Texture();

        void activate() {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, id);
        }

        // call before draw loop; resets active texture to 0
        static void beginDraw() {GLTexture::activeTexture = 0;}
    };
}

#endif
