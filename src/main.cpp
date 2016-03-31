#include <window/window.h>
#include <log/log.h>

int main(int argc, char **argv) {
    try {
        window::Window window;
        window.activate();
        // game loop
        while (window.running()) {
            window.clear();
            // draw
            // swap buffers
            window.swapBuffers();
            window.waitEvents();
        }
    } catch (const window::WindowCreationError &e) {
        LOG_ERR(e.what());
        return 1;
    } catch (const std::exception &e) {
        LOG_ERR("Critical failure: ", e.what());
        return 1;
    }
    return 0;
}
