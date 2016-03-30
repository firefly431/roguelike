#ifndef ROGUELIKE_INCLUDED_WINDOW_GLFW
#define ROGUELIKE_INCLUDED_WINDOW_GLFW

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "constants.h"

namespace window {
    class GLFWWindow {
        static bool errored;
        GLFWwindow *window;

        static void error_callback(int error, const char *description);
        static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
        static void char_callback(GLFWwindow *window, unsigned int codepoint);
    public:
        GLFWWindow(/* StateManager manager */); // activates window
        ~GLFWWindow() {glfwDestroyWindow(window); glfwTerminate();}

        void activate() {glfwMakeContextCurrent(window);}
        bool running() {return !GLFWWindow::errored && !glfwWindowShouldClose(window);}
        void clear() {glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);} // maybe move to graphics namespace
        void swapBuffers() {glfwSwapBuffers(window);}
        void waitEvents() {glfwWaitEvents();}
    };
}

#endif