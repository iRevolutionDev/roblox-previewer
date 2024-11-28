#pragma once
#include "drawable.hpp"
#include <bgfx/bgfx.h>

class cube : public drawable {
public:
    void init() override;
    void render() override;
    void shutdown() override;

private:
    bgfx::VertexBufferHandle m_vbh;
    bgfx::IndexBufferHandle m_ibh;
    bgfx::ProgramHandle m_program;
};