#include <graphics/glsprite.h>

namespace graphics {
    GLSprite::GLSprite(const GLTexture &tex, int width, int height, float tx, float ty, float tw, float th) : tex(tex), width(width), height(height), data() {
        data[2] = width;
        data[3] = height;
        setTexCoords(tx, ty, tw, th);
    }

    void GLSprite::draw(const GLRenderContext &ctx) const {
        tex.activate();
        // upload data
        ctx.activateVBO();
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof vx_data, vx_data);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    void GLSprite::setTexCoords(float tx, float ty, float tw, float th) {
        GLfloat *const tdata = reinterpret_cast<GLfloat *>(data + 8);
        float tr = tx + tw, tb = ty + th;
        tdata[0] = tx;
        tdata[1] = ty;
        tdata[2] = tr;
        tdata[3] = ty;
        tdata[4] = tx;
        tdata[5] = tb;
        tdata[6] = tr;
        tdata[7] = tb;
    }
}
