#pragma once

#include "application/application.hpp"
#include <GLFW/glfw3.h>

class glfw_application : public application {
public:
    glfw_application();

    ~glfw_application() override;

    void init(int32_t argc, char **argv) override;

    int shutdown() override;

    bool run() override;

    bool process_events() override;

    GLFWwindow *get_window() const override;

    bool should_close() const override;

    void swap_buffers() const;

    void poll_events() const;

    void *get_window_handle() const;

    void get_window_size(int &width, int &height) const;

private:
    GLFWwindow *m_window{nullptr};
};
