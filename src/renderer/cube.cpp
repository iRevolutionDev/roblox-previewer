#include "cube.hpp"
#include <bgfx/bgfx.h>
#include <bx/math.h>

#include "utils/shader_utils.hpp"

struct PosColorVertex {
    float x, y, z;
    uint32_t abgr;
};

static PosColorVertex s_cubeVertices[] = {
    {-1.0f, 1.0f, 1.0f, 0xff000000},
    {1.0f, 1.0f, 1.0f, 0xff0000ff},
    {-1.0f, -1.0f, 1.0f, 0xff00ff00},
    {1.0f, -1.0f, 1.0f, 0xff00ffff},
    {-1.0f, 1.0f, -1.0f, 0xffff0000},
    {1.0f, 1.0f, -1.0f, 0xffff00ff},
    {-1.0f, -1.0f, -1.0f, 0xffffff00},
    {1.0f, -1.0f, -1.0f, 0xffffffff},
};

static const uint16_t s_cubeIndices[] = {
    0, 1, 2, // front
    1, 3, 2,
    4, 6, 5, // back
    5, 6, 7,
    0, 2, 4, // left
    4, 2, 6,
    1, 5, 3, // right
    5, 7, 3,
    0, 4, 1, // top
    4, 5, 1,
    2, 3, 6, // bottom
    6, 3, 7,
};

static bgfx::VertexLayout s_vertexLayout;

void cube::init() {
    s_vertexLayout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true, true)
            .end();

    m_vbh = createVertexBuffer(
        bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)),
        s_vertexLayout
    );

    m_ibh = createIndexBuffer(
        bgfx::makeRef(s_cubeIndices, sizeof(s_cubeIndices))
    );

    const auto vs = shaders::create_shader("v_simple.bin", "vshader");
    const auto fs = shaders::create_shader("f_simple.bin", "fshader");
    m_program = createProgram(vs, fs, true);
}

void cube::render() {
    static float time = 0.0f;
    time += 0.01f;

    float mtx[16];
    bx::mtxRotateXY(mtx, time, time);
    mtx[12] = 0.0f; // x position
    mtx[13] = 0.0f; // y position
    mtx[14] = -5.0f; // z position

    bgfx::setTransform(mtx);

    setVertexBuffer(0, m_vbh);
    setIndexBuffer(m_ibh);

    submit(0, m_program);
}

void cube::shutdown() {
    destroy(m_vbh);
    destroy(m_ibh);
    destroy(m_program);
}
