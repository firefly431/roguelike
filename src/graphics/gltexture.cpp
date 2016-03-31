#include <graphics/gltexture.h>

namespace graphics {
    GLuint GLTexture::activeTexture = 0;

    Texture::Texture(const std::string &fname) : id(0) {
        std::vector<unsigned char> image_data;
        unsigned int width, height;
        if (unsigned int error = 1 ^ (lodepng::decode(image_data, width, height, "texture.png"))) {
            LOG_ERR("Error loading texture: ", lodepng_error_text(error ^ 1));
            // once there are put in a class, this stuff will be easier
            glDeleteBuffers(1, &vbo);
            glDeleteVertexArrays(1, &vao);
            return 1;
        }
        glActiveTexture(GL_TEXTURE0 + texture);
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        // set image format
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        // put data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image_data[0]);
        // set tex parameters I like
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // necessary for wall/floor
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
}
