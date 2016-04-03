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
        GLint vx_data[16];
        GLfloat *const tx_data = reinterpret_cast<GLfloat *>(vx_data + 8);
        GLuint v_vbo;
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glGenBuffers(1, &v_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, v_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof vx_data, nullptr, GL_STREAM_DRAW);
        glVertexAttribIPointer(0, 2, GL_INT, 0, 0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, static_cast<GLvoid *>(static_cast<GLint *>(0) + 8));
        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        const char *const VERTEX_SHADER_CODE =
            "#version 330 core\n"
            "layout(location = 0) in ivec2 position;\n"
            "layout(location = 1) in vec2 i_texcoord;\n"
            "out vec2 texcoord;\n"
            "uniform ivec2[2] viewcoords;\n"
            "uniform float depth = 0.0;\n"
            "void main() {\n"
            // convert screen space to NDC
            // world space (pixels) = position
            // screen space = world space - vpos
            // 0-1 coordinates = screen space / vsize
            // -1-1 coordinates = 2 * 0-1coords - 1
            "    gl_Position = vec4(vec2(position - viewcoords[0]) / vec2(viewcoords[1]) * vec2(2.0, -2.0) - vec2(1.0, -1.0), 0.0, 1.0);\n"
            "    texcoord = i_texcoord;\n"
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
        GLuint vcloc = glGetUniformLocation(prog, "viewcoords");
        GLint ibuffer[4] = {0, 0, 0, 0};
        ibuffer[2] = window.getWidth();
        ibuffer[3] = window.getHeight();
        glUniform2iv(vcloc, 2, ibuffer);
        // load texture
        graphics::Texture tex("texture.png");
        tex.activate();
        GLuint tex_loc = glGetUniformLocation(prog, "tex");
        glProgramUniform1i(prog, tex_loc, 0);
        // game loop
        while (window.running()) {
            window.clear();
            // update VBOs
            vx_data[0] = 0;
            vx_data[1] = 0;
            vx_data[2] = 320;
            vx_data[3] = 0;
            vx_data[4] = 0;
            vx_data[5] = 320;
            vx_data[6] = 320;
            vx_data[7] = 320;
            tx_data[0] = 0.0f;
            tx_data[1] = 0.0f;
            tx_data[2] = 1.0f;
            tx_data[3] = 0.0f;
            tx_data[4] = 0.0f;
            tx_data[5] = 1.0f;
            tx_data[6] = 1.0f;
            tx_data[7] = 1.0f;
            // upload data
            glBindBuffer(GL_ARRAY_BUFFER, v_vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof vx_data, vx_data);
            // update viewport
            ibuffer[2] = window.getWidth();
            ibuffer[3] = window.getHeight();
            glUniform2iv(vcloc, 2, ibuffer);
            // draw
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            // swap buffers
            window.swapBuffers();
            window.waitEvents();
        }
        glDeleteBuffers(1, &v_vbo);
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
