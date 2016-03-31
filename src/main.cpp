#include <window/window.h>
#include <log/log.h>

int main(int argc, char **argv) {
    try {
        window::Window window;
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
            "void main() {\n"
            "    gl_Position = vec4(position, 0.0, 1.0);\n"
            "}\n",
                   *const FRAGMENT_SHADER_CODE =
            "#version 330 core\n"
            "out vec4 color;\n"
            "void main() {\n"
            "    color = vec4(1.0, 0.0, 0.0, 1.0);\n"
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
        // game loop
        while (window.running()) {
            window.clear();
            // draw
            glDrawArrays(GL_TRIANGLES, 0, 3);
            // swap buffers
            window.swapBuffers();
            window.waitEvents();
        }
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    } catch (const window::WindowCreationError &e) {
        LOG_ERR(e.what());
        return 1;
    } catch (const std::exception &e) {
        LOG_ERR("Critical failure: ", e.what());
        return 1;
    }
    return 0;
}
