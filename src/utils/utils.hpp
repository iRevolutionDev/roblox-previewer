#pragma once

struct EngineArgs {
    EngineArgs(int _argc, const char *const*_argv);

    bgfx::RendererType::Enum m_type;
    uint16_t m_pciId;
};
