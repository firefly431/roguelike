#include <graphics/types.h>
#include <log/log.h>

#include <lodepng.h>

int main(int argc, char **argv) {
    try {
        graphics::Window window;
        window.activate();
        // initialize resources
        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        const GLfloat vx_data[] = {
             0.0f,  1.0f,
             1.0f, -1.0f,
            -1.0f, -1.0f,
        };
        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof vx_data, vx_data, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        const char *const VERTEX_SHADER_CODE =
            "#version 330 core\n"
            "layout(location = 0) in vec2 position;\n"
            "out vec2 texcoord;\n"
            "void main() {\n"
            "    gl_Position = vec4(position, 0.0, 1.0);\n"
            "    texcoord = position;\n"
            "}\n",
                   *const FRAGMENT_SHADER_CODE =
            "#version 330 core\n"
            "uniform sampler2D tex;\n"
            "in vec2 texcoord;\n"
            "out vec4 color;\n"
            "void main() {\n"
            "    color = texture(tex, texcoord);\n"
            "}\n";
        glShaderSource(vs, 1, &VERTEX_SHADER_CODE, NULL);
        glShaderSource(fs, 1, &FRAGMENT_SHADER_CODE, NULL);
        glCompileShader(vs);
        glCompileShader(fs);
        // TODO: check status and info log
        GLuint prog = glCreateProgram();
        glAttachShader(prog, vs);
        glAttachShader(prog, fs);
        glLinkProgram(prog);
        // TODO: check status and info log
        glDetachShader(prog, vs);
        glDetachShader(prog, fs);
        glDeleteShader(vs);
        glDeleteShader(fs);
        glUseProgram(prog);
        // load texture
        GLuint texture_id = 0;
        {
            std::vector<unsigned char> image_data;
            unsigned int width, height;
            if (unsigned int error = 1 ^ (lodepng::decode(image_data, width, height, "texture.png"))) {
                LOG_ERR("Error loading texture: ", lodepng_error_text(error ^ 1));
                // once there are put in a class, this stuff will be easier
                glDeleteBuffers(1, &vbo);
                glDeleteVertexArrays(1, &vao);
                return 1;
            }
            glActiveTexture(GL_TEXTURE0);
            glGenTextures(1, &texture_id);
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
        GLuint tex_loc = glGetUniformLocation(prog, "tex");
        glProgramUniform1i(prog, tex_loc, 0);
        // game loop
        while (window.running()) {
            window.clear();
            // draw
            glDrawArrays(GL_TRIANGLES, 0, 3);
            // swap buffers
            window.swapBuffers();
            window.waitEvents();
        }
        glDeleteTextures(1, &texture_id);
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    } catch (const graphics::WindowCreationError &e) {
        LOG_ERR(e.what());
        return 1;
    } catch (const std::exception &e) {
        LOG_ERR("Critical failure: ", e.what());
        return 1;
    }
    return 0;
}
