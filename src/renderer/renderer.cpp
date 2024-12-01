#include "renderer.hpp"
#include "application/application.hpp"
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <GLFW/glfw3.h>
#include "imgui.hpp"

renderer::renderer() = default;

renderer::~renderer() = default;

void renderer::init(const std::shared_ptr<application> &app, const bgfx::RendererType::Enum backend) {
    m_app = app;

    bgfx::Init init{};
    init.type = backend; // Set the backend type
    init.platformData.nwh = m_app->get_window_handle();

    int width, height;
    m_app->get_window_size(width, height);
    init.resolution.width = static_cast<uint32_t>(width);
    init.resolution.height = static_cast<uint32_t>(height);
    init.resolution.reset = BGFX_RESET_VSYNC;

    if (!bgfx::init(init)) {
        fprintf(stderr, "Failed to initialize bgfx\n");
        return;
    }

    bgfx::setDebug(BGFX_DEBUG_TEXT);
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
    bgfx::setViewRect(0, 0, 0, static_cast<uint16_t>(width), static_cast<uint16_t>(height));

    m_camera.set_perspective(60.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    m_imgui = std::make_shared<imgui>();
}

void renderer::shutdown() const {
    for (const auto &obj: m_drawables) {
        obj->shutdown();
    }

    m_imgui->shutdown();
    bgfx::shutdown();

    ImGui::DestroyContext();
}

void renderer::render_frame() {
    int width, height;
    m_app->get_window_size(width, height);
    bgfx::reset(static_cast<uint32_t>(width), static_cast<uint32_t>(height), BGFX_RESET_VSYNC);

    handle_input();

    for (const auto &obj: m_drawables) {
        obj->render();
    }

    m_imgui->reset(static_cast<uint16_t>(width), static_cast<uint16_t>(height));
    m_imgui->new_frame();
    ImGui::ShowDemoWindow();
    ImGui::Render();
    if (auto *draw_data = ImGui::GetDrawData()) {
        m_imgui->render(draw_data);
    }

    bgfx::setViewTransform(0, m_camera.get_view_matrix(), m_camera.get_proj_matrix());

    bgfx::touch(0);

    bgfx::dbgTextClear();
    bgfx::dbgTextPrintf(0, 0, 0x0f, "Roblox Previewer - Debug Info");

    bgfx::setViewRect(0, 0, 0, static_cast<uint16_t>(width), static_cast<uint16_t>(height));
    const bgfx::Stats *stats = bgfx::getStats();
    bgfx::dbgTextPrintf(0, 3, 0x0f, "GPU: %s", bgfx::getRendererName(bgfx::getRendererType()));
    bgfx::dbgTextPrintf(0, 4, 0x0f, "Resolution: %d x %d", stats->width, stats->height);
    bgfx::dbgTextPrintf(0, 5, 0x0f, "Draw Calls: %d", stats->numDraw);
    bgfx::dbgTextPrintf(0, 6, 0x0f, "Compute Calls: %d", stats->numCompute);
    bgfx::dbgTextPrintf(0, 7, 0x0f, "Primitives: %d", stats->numPrims);
    bgfx::dbgTextPrintf(0, 8, 0x0f, "Textures: %d", stats->numTextures);
    bgfx::dbgTextPrintf(0, 9, 0x0f, "Shaders: %d", stats->numShaders);
    bgfx::dbgTextPrintf(0, 11, 0x0f, "Frame Time: %.3f ms", stats->cpuTimeFrame / 1000.0f);

    bgfx::frame();
}

void renderer::add_drawable(const std::shared_ptr<drawable> &obj) {
    m_drawables.push_back(obj);
    obj->init();
}

camera &renderer::get_camera() {
    return m_camera;
}

void renderer::handle_input() {
    ImGuiIO &io = ImGui::GetIO();
    int width, height;
    m_app->get_window_size(width, height);
    io.DisplaySize = ImVec2(static_cast<float>(width), static_cast<float>(height));

    double mouse_x, mouse_y;
    glfwGetCursorPos(m_app->get_window(), &mouse_x, &mouse_y);
    io.MousePos = ImVec2(static_cast<float>(mouse_x), static_cast<float>(mouse_y));

    for (int i = 0; i < 3; i++) {
        io.MouseDown[i] = glfwGetMouseButton(m_app->get_window(), i) == GLFW_PRESS;
    }

    if (glfwGetKey(m_app->get_window(), GLFW_KEY_W) == GLFW_PRESS) {
        m_camera.move_forward(0.1f);
    }
    if (glfwGetKey(m_app->get_window(), GLFW_KEY_S) == GLFW_PRESS) {
        m_camera.move_backward(0.1f);
    }
    if (glfwGetKey(m_app->get_window(), GLFW_KEY_A) == GLFW_PRESS) {
        m_camera.move_left(0.1f);
    }
    if (glfwGetKey(m_app->get_window(), GLFW_KEY_D) == GLFW_PRESS) {
        m_camera.move_right(0.1f);
    }
    if (glfwGetKey(m_app->get_window(), GLFW_KEY_Q) == GLFW_PRESS) {
        m_camera.move_up(0.1f);
    }
    if (glfwGetKey(m_app->get_window(), GLFW_KEY_E) == GLFW_PRESS) {
        m_camera.move_down(0.1f);
    }
}
