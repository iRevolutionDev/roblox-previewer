#include "common.hpp"

#include "imgui.hpp"
#include "utils/shader_utils.hpp"

#include "bx/math.h"

imgui::imgui() {
    g_imgui = this;

    init();
}

imgui::~imgui() {
    ImGui::DestroyContext();
    g_imgui = nullptr;
}

void imgui::init() {
    const auto &io = ImGui::GetIO();

    m_vertex_layout.begin()
            .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .end();

    io.Fonts->AddFontDefault();
    update_font_texture();
    m_texture_uniform = createUniform("s_tex", bgfx::UniformType::Sampler);

    const auto vs_handle = shaders::create_shader("v_imgui.bin", "v_imgui");
    const auto fs_handle = shaders::create_shader("f_imgui.bin", "f_imgui");
    m_program = createProgram(vs_handle, fs_handle, true);
}

void imgui::shutdown() {
    destroy(m_program);
    destroy(m_font_texture);
    destroy(m_texture_uniform);
}

void imgui::reset(const uint16_t width, const uint16_t height) {
    ImGuiIO &io = ImGui::GetIO();
    io.DisplaySize = ImVec2(width, height);
    update_font_texture();
}

void imgui::new_frame() {
    ImGui::NewFrame();
}

void imgui::render(ImDrawData *draw_data) {
    const auto &io = ImGui::GetIO();
    const int width = static_cast<int>(io.DisplaySize.x * io.DisplayFramebufferScale.x);
    const int height = static_cast<int>(io.DisplaySize.y * io.DisplayFramebufferScale.y);
    if (width == 0 || height == 0) {
        return;
    }

    draw_data->ScaleClipRects(io.DisplayFramebufferScale);

    constexpr auto state =
            BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_MSAA |
            BGFX_STATE_BLEND_FUNC(
                BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA);

    const bgfx::Caps *caps = bgfx::getCaps();

    float ortho[16];
    bx::mtxOrtho(
        ortho, 0.0f, io.DisplaySize.x, io.DisplaySize.y, 0.0f, 0.0f, 1000.0f,
        0.0f, caps->homogeneousDepth);
    bgfx::setViewTransform(VIEW_ID, nullptr, ortho);
    bgfx::setViewRect(VIEW_ID, 0, 0, static_cast<uint16_t>(width), static_cast<uint16_t>(height));

    for (int n = 0; n < draw_data->CmdListsCount; n++) {
        const ImDrawList *cmd_list = draw_data->CmdLists[n];

        bgfx::TransientVertexBuffer tvb{};
        bgfx::TransientIndexBuffer tib{};

        const auto numVertices = static_cast<uint32_t>(cmd_list->VtxBuffer.size());
        const auto numIndices = static_cast<uint32_t>(cmd_list->IdxBuffer.size());

        if (numVertices != getAvailTransientVertexBuffer(
                numVertices, m_vertex_layout) ||
            numIndices != bgfx::getAvailTransientIndexBuffer(numIndices)) {
            // not enough space in transient buffer, quit drawing the rest...
            break;
        }

        allocTransientVertexBuffer(&tvb, numVertices, m_vertex_layout);
        allocTransientIndexBuffer(&tib, numIndices);

        auto *verts = reinterpret_cast<ImDrawVert *>(tvb.data);
        memcpy(
            verts, cmd_list->VtxBuffer.begin(),
            numVertices * sizeof(ImDrawVert));

        auto *indices = reinterpret_cast<ImDrawIdx *>(tib.data);
        memcpy(
            indices, cmd_list->IdxBuffer.begin(),
            numIndices * sizeof(ImDrawIdx));

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++) {
            const ImDrawCmd *pcmd = &cmd_list->CmdBuffer[cmd_i];

            if (pcmd->UserCallback) {
                pcmd->UserCallback(cmd_list, pcmd);
            } else {
                const auto xx = static_cast<uint16_t>(bx::max(pcmd->ClipRect.x, 0.0f));
                const auto yy = static_cast<uint16_t>(bx::max(pcmd->ClipRect.y, 0.0f));
                bgfx::setScissor(
                    xx, yy, static_cast<uint16_t>(bx::min(pcmd->ClipRect.z, 65535.0f)) - xx,
                    static_cast<uint16_t>(bx::min(pcmd->ClipRect.w, 65535.0f)) - yy);

                bgfx::setState(state);
                const bgfx::TextureHandle texture = {
                    static_cast<uint16_t>(static_cast<intptr_t>(pcmd->TextureId) & 0xffff)
                };
                setTexture(0, m_texture_uniform, texture);
                setVertexBuffer(0, &tvb, 0, numVertices);
                setIndexBuffer(&tib, pcmd->IdxOffset, pcmd->ElemCount);
                submit(VIEW_ID, m_program);
            }
        }
    }

    ImGui::Render();
}

void imgui::update_font_texture() {
    unsigned char *data;
    int width, height;
    ImGui::GetIO().Fonts->GetTexDataAsRGBA32(&data, &width, &height);
    if (isValid(m_font_texture)) {
        destroy(m_font_texture);
    }
    m_font_texture = createTexture2D(static_cast<uint16_t>(width), static_cast<uint16_t>(height), false, 1,
                                     bgfx::TextureFormat::BGRA8, 0, bgfx::copy(data, width * height * 4));
    ImGui::GetIO().Fonts->SetTexID(
        reinterpret_cast<ImTextureID>(reinterpret_cast<void *>(static_cast<uintptr_t>(m_font_texture.idx))));
}
