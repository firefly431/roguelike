#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "log/log.h"

const int DEFAULT_WIDTH = 640;
const int DEFAULT_HEIGHT = 480;
const char *const WINDOW_TITLE = "Roguelike";

static bool errored = false;

static void error_callback(int error, const char *description) {
    LOG_ERR("GLFW error: ", description);
    errored = true;
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // do nothing for now
}

static void char_callback(GLFWwindow *window, int codepoint) {
    // do nothing for now
}

int main(int argc, char **argv) {
    // initialize GLFW
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        LOG_ERR("Failed to initialize GLFW");
        return 1;
    }
    // set up window
    GLFWwindow *window = glfwCreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (!window) {
        LOG_ERR("Failed to create window");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetKeyCallback(window, key_callback);
    // initialize GLEW
    glewExperimental = true;
    if (auto error = glewInit() ^ GLEW_OK) {
        LOG_ERR("Failed to initialize GLEW: ", glewGetErrorString(error ^ GLEW_OK));
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }
    // game loop
    while (!glfwWindowShouldClose(window) && !errored) {
        glClear(GL_COLOR_BUFFER_BIT);
        // draw
        // swap buffers
        glfwSwapBuffers(window);
        glfwWaitEvents();
    }
    // cleanup
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
