#include "common.hpp"
#include "engine.hpp"
#include "utils/utils.hpp"

#ifdef RBX_PREVIEWER_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#endif

#include <bgfx/bgfx.h>

#include "application/frontend/glfw/glfw_application.hpp"
#include "application/application.hpp"
#include "renderer/renderer.hpp"
#include "renderer/cube.hpp"
#include "renderer/camera.hpp"

engine::engine() {
    g_engine = this;
    m_app = std::make_shared<glfw_application>();
    m_renderer = std::make_unique<renderer>();
}

engine::~engine() {
    g_engine = nullptr;
}

void engine::init(int32_t argc, char **argv) {
    m_app->init(argc, argv);
    m_renderer->init(m_app, bgfx::RendererType::Count);

    auto cube_obj = std::make_shared<cube>();
    add_drawable(cube_obj);

    // Set up the camera temporary bloated code ¯\_(ツ)_/¯
    auto &cam = m_renderer->get_camera();
    cam.set_position({0.0f, 0.0f, 5.0f});
    cam.look_at({0.0f, 0.0f, 0.0f});
}

int engine::shutdown() {
    m_renderer->shutdown();
    return m_app->shutdown();
}

bool engine::run() {
    while (!m_app->should_close()) {
        if (!process_events()) {
            return false;
        }

        m_renderer->render_frame();
        m_app->poll_events();
    }

    shutdown();
    return true;
}

bool engine::process_events() {
    return m_app->process_events();
}

void engine::add_drawable(std::shared_ptr<drawable> obj) {
    m_drawables.push_back(obj);
    m_renderer->add_drawable(obj);
}

camera &engine::get_camera() const {
    return m_renderer->get_camera();
}
