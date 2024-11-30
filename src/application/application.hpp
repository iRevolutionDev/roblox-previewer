#pragma once

#include "GLFW/glfw3.h"

class application {
public:
    application();

    virtual ~application();

    virtual void init(int32_t argc, char **argv) = 0;

    virtual int shutdown() = 0;

    virtual bool run() = 0;

    virtual bool process_events() = 0;

    virtual void *get_window_handle() const = 0;

    virtual void get_window_size(int &width, int &height) const = 0;

    virtual bool should_close() const = 0;

    virtual void swap_buffers() const = 0;

    virtual void poll_events() const = 0;

    virtual GLFWwindow *get_window() const = 0;

    void set_window_title(const char *title);

    void set_window_size(uint32_t width, uint32_t height);

protected:
    uint32_t m_width{1024};
    uint32_t m_height{768};
    const char *m_window_title{"Roblox Previewer"};
};
