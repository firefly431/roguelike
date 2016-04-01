#ifndef ROGUELIKE_INCLUDED_GRAPHICS_WINDOW
#define ROGUELIKE_INCLUDED_GRAPHICS_WINDOW

#include <graphics/glfwwindow.h>
#include <string>
#include <stdexcept>

namespace graphics {
    class WindowCreationError : public std::runtime_error {
    public:
        explicit WindowCreationError(const char *what) : std::runtime_error(what) {}
        explicit WindowCreationError(const std::string &what) : std::runtime_error(what) {}
    };
    class TextureCreationError : public std::runtime_error {
    public:
        explicit TextureCreationError(const char *what) : std::runtime_error(what) {}
        explicit TextureCreationError(const std::string &what) : std::runtime_error(what) {}
    };
}

#endif
