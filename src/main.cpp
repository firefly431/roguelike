#include <graphics/types.h>
#include <graphics/exceptions.h>
#include <log/log.h>

#include <lodepng.h>

#include <cmath>

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
        tex.activate();
        // make sprite
        graphics::Sprite sprite(tex, 320, 320, 0.0f, 0.0f, 1.0f, 1.0f);
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
            sprite.setPosition(static_cast<int>(std::cos(t) * 100.0f + 100.0f), static_cast<int>(std::sin(t) * 100.0f + 100.0f));
            sprite.draw(ctx);
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
