#include <graphics/glrenderctx.h>

namespace graphics {
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

    GLRenderContext::GLRenderContext() : vao(0), vbo(0), prog(0), vcloc(0) {
        // initialize resources
        // VAO
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        // VBO and attributes
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat[16]), nullptr, GL_STREAM_DRAW);
        glVertexAttribIPointer(0, 2, GL_INT, 0, 0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, static_cast<GLvoid *>(static_cast<GLint *>(0) + 8));
        // shaders
        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(vs, 1, &VERTEX_SHADER_CODE, NULL);
        glShaderSource(fs, 1, &FRAGMENT_SHADER_CODE, NULL);
        glCompileShader(vs);
        glCompileShader(fs);
        // TODO: check status and info log
        prog = glCreateProgram();
        glAttachShader(prog, vs);
        glAttachShader(prog, fs);
        glLinkProgram(prog);
        // TODO: check status and info log
        glDetachShader(prog, vs);
        glDetachShader(prog, fs);
        glDeleteShader(vs);
        glDeleteShader(fs);
        glUseProgram(prog);
        // get uniform locations
        vcloc = glGetUniformLocation(prog, "viewcoords");
        GLuint tex_loc = glGetUniformLocation(prog, "tex");
        // and also bind texture unit 0
        glProgramUniform1i(prog, tex_loc, 0);
    }
    GLRenderContext::~GLRenderContext() {
        glDeleteProgram(prog);
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }
}
