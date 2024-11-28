#pragma once

class engine {
public:
    engine();

    ~engine();

    void init(int32_t argc, char **argv) const;

private:
    uint32_t m_width{800};
    uint32_t m_height{600};
    uint32_t m_debug{BGFX_DEBUG_TEXT};
    uint32_t m_reset{BGFX_RESET_VSYNC};

    bgfx::VertexBufferHandle m_vbh{};
    bgfx::IndexBufferHandle m_ibh{};
    bgfx::ProgramHandle m_program{};
};

inline engine *g_engine{};
