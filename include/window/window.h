#ifndef ROGUELIKE_INCLUDED_WINDOW_WINDOW
#define ROGUELIKE_INCLUDED_WINDOW_WINDOW

#include <window/glfw.h>
#include <string>
#include <stdexcept>

namespace window {
    typedef GLFWWindow Window;

    class WindowCreationError : public std::runtime_error {
    public:
        explicit WindowCreationError(const char *what) : std::runtime_error(what) {}
        explicit WindowCreationError(const std::string &what) : std::runtime_error(what) {}
    };
}

#endif
