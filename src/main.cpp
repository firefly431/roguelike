#include <graphics/types.h>
#include <graphics/exceptions.h>
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
            0.0f, 0.0f,
            1.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
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
            "uniform ivec2[2] viewcoords;\n"
            "uniform ivec2[2] objcoords;\n"
            "uniform vec2[2] texcoords;\n"
            "uniform float depth = 0.0;\n"
            "void main() {\n"
            // convert screen space to NDC
            // world space (pixels) = opos + position * osize
            // screen space = world space - vpos
            // 0-1 coordinates = screen space / vsize
            // -1-1 coordinates = 2 * 0-1coords - 1
            "    gl_Position = vec4((objcoords[0] - viewcoords[0] + position * objcoords[1]) / vec2(viewcoords[1]) * 2.0 - 1.0, 0.0, 1.0);\n"
            "    gl_Position.y = -gl_Position.y;"
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
        // set uniforms
        GLuint vcloc = glGetUniformLocation(prog, "viewcoords"),
               ocloc = glGetUniformLocation(prog, "objcoords"),
               tcloc = glGetUniformLocation(prog, "texcoords");
        GLint ibuffer[4] = {0, 0, 0, 0};
        ibuffer[2] = window.getWidth();
        ibuffer[3] = window.getHeight();
        glUniform2iv(vcloc, 2, ibuffer);
        ibuffer[2] = ibuffer[3] = 320;
        glUniform2iv(ocloc, 2, ibuffer);
        GLfloat fbuffer[4] = {0.0f, 0.0f, 1.0f, 1.0f};
        glUniform2fv(tcloc, 2, fbuffer);
        // load texture
        graphics::Texture tex("texture.png");
        tex.activate();
        GLuint tex_loc = glGetUniformLocation(prog, "tex");
        glProgramUniform1i(prog, tex_loc, 0);
        // game loop
        while (window.running()) {
            window.clear();
            // update uniform
            ibuffer[2] = window.getWidth();
            ibuffer[3] = window.getHeight();
            glUniform2iv(vcloc, 2, ibuffer);
            // draw
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            // swap buffers
            window.swapBuffers();
            window.waitEvents();
        }
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
