#include <graphics/types.h>
#include <graphics/exceptions.h>
#include <log/log.h>

#include <lodepng.h>

#include <cmath>
#include <cstdlib>
#include <vector>

int main(int argc, char **argv) {
    try {
        graphics::Window window;
        window.activate();
        graphics::RenderContext ctx;
        GLint ibuffer[4] = {0, 0, 0, 0};
        ibuffer[2] = window.getWidth();
        ibuffer[3] = window.getHeight();
        glUniform2iv(ctx.getViewportLocation(), 2, ibuffer);
        // load texture
        graphics::Texture tex("texture.png");
        graphics::Texture ts("tileset.png");
        // make sprite
        graphics::Sprite sprite(tex, 320, 320, 0.0f, 0.0f, 1.0f, 1.0f);
        // make tileset data
        generation::Dungeon dungeon(17, 15);
        dungeon.generate(5, 500);
        auto map_data = dungeon.getTiles();
        graphics::Tilemap map(std::begin(map_data), std::end(map_data), dungeon.getWidth(), dungeon.getHeight(), 8, 8, 1, 2, ts, ctx);
        // simple animation
        float t = 0.0f;
        // game loop
        while (window.running()) {
            // update viewport
            ibuffer[2] = window.getWidth();
            ibuffer[3] = window.getHeight();
            glUniform2iv(ctx.getViewportLocation(), 2, ibuffer);
            // draw
            window.clear();
            graphics::Texture::beginDraw();
            // draw tileset
            map.draw();
            // draw sprite
            sprite.setPosition(static_cast<int>(std::cos(t) * 100.0f + 100.0f), static_cast<int>(std::sin(t) * 100.0f + 100.0f));
            ctx.activateVAO();
            ctx.activateVBO();
            sprite.draw();
            t += 0.05;
            // swap buffers
            window.swapBuffers();
            window.waitEvents();
        }
    } catch (const graphics::WindowCreationError &e) {
        LOG_ERR(e.what());
        return 1;
    } catch (const std::exception &e) {
        LOG_ERR("Critical failure: ", e.what());
        return 1;
    }
    return 0;
}
