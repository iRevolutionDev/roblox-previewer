#pragma once
#include <memory>
#include <vector>

class application;
class renderer;
class drawable;
class camera;

class engine {
public:
    engine();
    ~engine();
    void init(int32_t argc, char **argv);
    int shutdown();
    bool run();
    bool process_events();
    void add_drawable(std::shared_ptr<drawable> obj);
    camera& get_camera();

private:
    std::shared_ptr<application> m_app;
    std::unique_ptr<renderer> m_renderer;
    std::vector<std::shared_ptr<drawable>> m_drawables;
};

inline engine *g_engine{};
