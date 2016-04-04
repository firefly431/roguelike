#ifndef ROGUELIKE_INCLUDED_GRAPHICS_GLRENDERCTX
#define ROGUELIKE_INCLUDED_GRAPHICS_GLRENDERCTX

#include <GL/glew.h>

namespace graphics {
    class GLTexture;
    // all methods of this class must be called with the OpenGL
    // context active.
    // Holds the global OpenGL objects used for rendering.
    // Not the most well-designed of classes, but should be rather
    // easy to extend as the need arises.
    class GLRenderContext {
        // vbo is 16 GLfloats/GLints
        GLuint vao, vbo;
        GLuint prog;
        GLuint vcloc;
    public:
        GLRenderContext();
        ~GLRenderContext();
        void activateVAO() const {glBindVertexArray(vao);}
        static void deactivateVAO() const {glBindVertexArray(0);}
        void activateVBO() const {glBindBuffer(GL_ARRAY_BUFFER, vbo);}
        static void deactivateVBO() const {glBindBuffer(GL_ARRAY_BUFFER, 0);}
        void activateProgram() const {glUseProgram(prog);}
        static void deactivateProgram() const {glUseProgram(0);}
        GLuint getViewportLocation() const {return vcloc;}
    };
}

#endif
