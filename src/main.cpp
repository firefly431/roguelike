#include <window/window.h>

int main(int argc, char **argv) {
    try {
        GLFWWindow window;
    } catch (const std::exception &e) {
        // TODO: log the exception
        return 1;
    }
    // game loop
    while (window.running()) {
        window.clear();
        // draw
        // swap buffers
        window.swapBuffers();
    }
    return 0;
}
