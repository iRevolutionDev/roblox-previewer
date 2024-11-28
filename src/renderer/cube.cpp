#include "cube.hpp"
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/math.h>

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

static auto vs_shader_code = R"(
    #version 330 core
    uniform mat4 u_modelViewProj;
    in vec3 a_position;
    in vec4 a_color0;
    out vec4 v_color;
    void main() {
        gl_Position = u_modelViewProj * vec4(a_position, 1.0);
        v_color = a_color0;
    }
)";

static auto fs_shader_code = R"(
    #version 330 core
    in vec4 v_color;
    out vec4 fragColor;
    void main() {
        fragColor = v_color;
    }
)";

bgfx::ShaderHandle load_shader(const char *shader_code, const char *name) {
    const bgfx::Memory *mem = bgfx::copy(shader_code, strlen(shader_code) + 1);
    return bgfx::createShader(mem);
}

void cube::init() {
    s_vertexLayout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true, true)
            .end();

    m_vbh = bgfx::createVertexBuffer(
        bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)),
        s_vertexLayout
    );

    m_ibh = bgfx::createIndexBuffer(
        bgfx::makeRef(s_cubeIndices, sizeof(s_cubeIndices))
    );

    bgfx::ShaderHandle vs = load_shader(vs_shader_code, "vs_cube");
    bgfx::ShaderHandle fs = load_shader(fs_shader_code, "fs_cube");
    m_program = bgfx::createProgram(vs, fs, true);
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

    bgfx::setVertexBuffer(0, m_vbh);
    bgfx::setIndexBuffer(m_ibh);

    bgfx::submit(0, m_program);
}

void cube::shutdown() {
    bgfx::destroy(m_vbh);
    bgfx::destroy(m_ibh);
    bgfx::destroy(m_program);
}
