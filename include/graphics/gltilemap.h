#ifndef ROGUELIKE_INCLUDED_GLTILEMAP
#define ROGUELIKE_INCLUDED_GLTILEMAP

#include <vector>
#include <iterator>
#include <cstdlib>

#include <graphics/gltexture.h>

namespace graphics {
    class GLTilemap {
        typedef unsigned char Index; // no one's going to use more than 256 tiles, right?
        GLTexture &tex;
        unsigned int width, height;
        std::vector<Index> map;
        GLuint vao, vbo, ibo;
    public:
        template<typename Iterator> GLTilemap(Iterator begin, Iterator end, unsigned int width, unsigned int height, unsigned int cell_width, unsigned int cell_height, unsigned int tex_rows, unsigned int tex_cols, GLTexture &tex, const GLRenderContext &ctx) : tex(tex), width(width), height(height), map(begin, end), vao(0), vbo(0), ibo(0) {
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);
            // also enable the same shader
            ctx.activateProgram();
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glGenBuffers(1, &vbo);
            glGenBuffers(1, &ibo);
            // generate vertex data
            // width * height * 4 corners/cell * (2 ints + 2 floats)/corner * 4 bytes = width * height * 64
            {
                GLint *map_data = static_cast<GLint *>(std::malloc(width * height * 64));
                const float tex_width = 1.0f / tex_cols, tex_height = 1.0f / tex_rows;
                for (unsigned int i = 0; i < width * height; i++) {
                    unsigned int y = i / width, x = i % width;
                    unsigned int tx = 0, ty = 0;
                    char c = map[i];
                    tx = c % tex_cols;
                    ty = c / tex_cols;
                    map_data[i * 16     ] = x * cell_width;
                    map_data[i * 16 +  1] = y * cell_height;
                    *reinterpret_cast<GLfloat *>(&map_data[i * 16 +  2]) = tx * tex_width;
                    *reinterpret_cast<GLfloat *>(&map_data[i * 16 +  3]) = ty * tex_height;
                    map_data[i * 16 +  4] = x * cell_width  + cell_width;
                    map_data[i * 16 +  5] = y * cell_height;
                    *reinterpret_cast<GLfloat *>(&map_data[i * 16 +  6]) = tx * tex_width  + tex_width;
                    *reinterpret_cast<GLfloat *>(&map_data[i * 16 +  7]) = ty * tex_height;
                    map_data[i * 16 +  8] = x * cell_width;
                    map_data[i * 16 +  9] = y * cell_height + cell_height;
                    *reinterpret_cast<GLfloat *>(&map_data[i * 16 + 10]) = tx * tex_width;
                    *reinterpret_cast<GLfloat *>(&map_data[i * 16 + 11]) = ty * tex_height + tex_height;
                    map_data[i * 16 + 12] = x * cell_width  + cell_width;
                    map_data[i * 16 + 13] = y * cell_height + cell_height;
                    *reinterpret_cast<GLfloat *>(&map_data[i * 16 + 14]) = tx * tex_width  + tex_width;
                    *reinterpret_cast<GLfloat *>(&map_data[i * 16 + 15]) = ty * tex_height + tex_height;
                }
                // upload data
                glBindBuffer(GL_ARRAY_BUFFER, vbo);
                glBufferData(GL_ARRAY_BUFFER, width * height * 64, map_data, GL_STATIC_DRAW);
                // set vertex data
                glVertexAttribIPointer(0, 2, GL_INT, sizeof(GLint[4]), 0);
                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLint[4]), static_cast<GLvoid *>(static_cast<GLint *>(0) + 2));
                // IBO data is pretty easy
                // # vertices = 3 verts/tri * 2 tris/quad * (width * height) quads
                //            = width * height * 6
                // bytes = width * height * 24
                // triangle diagram:
                // 0---1
                // |  /|
                // | / |
                // |/  |
                // 2---3
                // Tri 1: 012
                // Tri 2: 123
                std::free(map_data);
                GLuint *idx_data = static_cast<GLuint *>(std::malloc(width * height * 24));
                for (unsigned int i = 0; i < width * height; i++) {
                    idx_data[i * 6    ] = i * 4;
                    idx_data[i * 6 + 1] = i * 4 + 1;
                    idx_data[i * 6 + 2] = i * 4 + 2;
                    idx_data[i * 6 + 3] = i * 4 + 1;
                    idx_data[i * 6 + 4] = i * 4 + 2;
                    idx_data[i * 6 + 5] = i * 4 + 3;
                }
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, width * height * 24, idx_data, GL_STATIC_DRAW);
                std::free(idx_data);
            }
        }

        void draw() {
            glBindVertexArray(vao);
            tex.activate();
            glDrawElements(GL_TRIANGLES, width * height * 6, GL_UNSIGNED_INT, 0);
        }
    };
}

#endif
