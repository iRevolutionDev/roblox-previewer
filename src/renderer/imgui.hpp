#pragma once

#define VIEW_ID 255

class imgui {
public:
    imgui();

    ~imgui();

    void init();

    void shutdown();

    void reset(uint16_t width, uint16_t height);

    void render(ImDrawData *draw_data);

    void new_frame();

    void update_font_texture();

private:
    bgfx::VertexLayout m_vertex_layout{};
    bgfx::ProgramHandle m_program{};
    bgfx::TextureHandle m_font_texture{};
    bgfx::UniformHandle m_texture_uniform{};
};

inline imgui *g_imgui{};
