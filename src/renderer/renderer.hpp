#pragma once
#include <memory>
#include <vector>
#include "drawable.hpp"
#include "camera.hpp"

class application;

class renderer {
public:
    renderer();
    ~renderer();
    void init(const std::shared_ptr<application> &app, bgfx::RendererType::Enum backend);
    void shutdown() const;
    void render_frame();
    void add_drawable(const std::shared_ptr<drawable>& obj);
    camera& get_camera();
    void handle_input();

private:
    std::shared_ptr<application> m_app;
    std::vector<std::shared_ptr<drawable>> m_drawables;
    camera m_camera;
};
