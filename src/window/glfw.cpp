#include <window/window.h>

#include <log/log.h>

using window::GLFWWindow;

static void error_callback(int error, const char *description) {
    LOG_ERR("GLFW error: ", description);
    GLFWWindow::errored = true;
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // do nothing for now
}

static void char_callback(GLFWwindow *window, int codepoint) {
    // do nothing for now
}

GLFWWindow::GLFWWindow() : errored(false), window(nullptr) {
    // initialize GLFW
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        throw WindowCreationError("Failed to initialize GLFW");
    }
    // set up window
    ::GLFWwindow *window = glfwCreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (!window) {
        glfwTerminate();
        throw WindowCreationError("Failed to create window");
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetKeyCallback(window, key_callback);
    // initialize GLEW
    ::glewExperimental = true;
    if (auto error = glewInit() ^ GLEW_OK) {
        glfwDestroyWindow(window);
        glfwTerminate();
        throw WindowCreationError(std::string("Failed to initialize GLEW: ") + glewGetErrorString(error ^ GLEW_OK));
    }
}
