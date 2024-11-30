#include "common.hpp"
#include "glfw_application.hpp"
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#ifdef RBX_PREVIEWER_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#endif

#include <GLFW/glfw3native.h>

static bool s_showStats = false;

static void glfw_errorCallback(int error, const char *description) {
    fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

static void glfw_keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_F1 && action == GLFW_RELEASE)
        s_showStats = !s_showStats;
}

glfw_application::glfw_application() : application() {
}

glfw_application::~glfw_application() {
}

void glfw_application::init(int32_t argc, char **argv) {
    glfwSetErrorCallback(glfw_errorCallback);
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    m_window = glfwCreateWindow(m_width, m_height, m_window_title, nullptr, nullptr);
    if (!m_window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return;
    }

    glfwSetKeyCallback(m_window, glfw_keyCallback);

    bgfx::renderFrame();
}

int glfw_application::shutdown() {
    if (m_window) {
        glfwDestroyWindow(m_window);
    }

    bgfx::renderFrame();
    glfwTerminate();
    return 0;
}

bool glfw_application::run() {
    while (!glfwWindowShouldClose(m_window)) {
        if (!process_events()) {
            return false;
        }

        // Poll events
        glfwPollEvents();
    }
    return true;
}

bool glfw_application::process_events() {
    glfwPollEvents();

    if (glfwWindowShouldClose(m_window)) {
        return false;
    }

    return true;
}

GLFWwindow *glfw_application::get_window() const {
    return m_window;
}

bool glfw_application::should_close() const {
    return glfwWindowShouldClose(m_window);
}

void glfw_application::swap_buffers() const {
    glfwSwapBuffers(m_window);
}

void glfw_application::poll_events() const {
    glfwPollEvents();
}

void *glfw_application::get_window_handle() const {
#if RBX_PREVIEWER_PLATFORM_PLATFORM_LINUX
    return (void*)(uintptr_t)glfwGetX11Window(m_window);
#elif RBX_PREVIEWER_PLATFORM_PLATFORM_OSX
    return glfwGetCocoaWindow(m_window);
#elif RBX_PREVIEWER_PLATFORM_WINDOWS
    return glfwGetWin32Window(m_window);
#endif
}

void glfw_application::get_window_size(int &width, int &height) const {
    glfwGetWindowSize(m_window, &width, &height);
}
