#ifndef ROGUELIKE_INCLUDED_GRAPHICS_GLTEXTURE
#define ROGUELIKE_INCLUDED_GRAPHICS_GLTEXTURE

#include <GL/glew.h>

#include <lodepng.h>

#include <string>

namespace graphics {
    class GLTexture {
        GLuint id;
    public:
        Texture(const std::string &fname);
        ~Texture();
        
        void activate();
    };
}

#endif
