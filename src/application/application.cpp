#include "application.hpp"

application::application() = default;
application::~application() = default;

void application::set_window_title(const char *title) {
    m_window_title = title;
}

void application::set_window_size(uint32_t width, uint32_t height) {
    m_width = width;
    m_height = height;
}
