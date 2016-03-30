#ifndef ROGUELIKE_INCLUDED_WINDOW_GLFW
#define ROGUELIKE_INCLUDED_WINDOW_GLFW

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "constants.h"

namespace window {
    class GLFWWindow {
        static bool errored;
        GLFWwindow *window;
    public:
        GLFWWindow(/* StateManager manager */); // activates window
        ~GLFWWindow() {glfwDestroyWindow(window); glfwTerminate();}

        void activate() {glfwMakeContextCurrent(window);}
        bool running() {return !GLFWWindow::errored && !glfwWindowShouldClose(window);}
        void clear() {glClear();} // maybe move to graphics namespace
        void swapBuffers() {glfwSwapBuffers();}
        void waitEvents() {glfwWaitEvents();}
    };
}

#endif
