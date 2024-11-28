#include "common.hpp"
#include "bx/commandline.h"
#include "utils.hpp"

EngineArgs::EngineArgs(int _argc, const char *const *_argv) {
    const bx::CommandLine cmdLine(_argc, _argv);

    if (cmdLine.hasArg("gl")) {
        m_type = bgfx::RendererType::OpenGL;
    } else if (cmdLine.hasArg("vk")) {
        m_type = bgfx::RendererType::Vulkan;
    } else if (cmdLine.hasArg("noop")) {
        m_type = bgfx::RendererType::Noop;
    } else if (cmdLine.hasArg("d3d11")) {
        m_type = bgfx::RendererType::Direct3D11;
    } else if (cmdLine.hasArg("d3d12")) {
        m_type = bgfx::RendererType::Direct3D12;
    }

    if (cmdLine.hasArg("amd")) {
        m_pciId = BGFX_PCI_ID_AMD;
    } else if (cmdLine.hasArg("nvidia")) {
        m_pciId = BGFX_PCI_ID_NVIDIA;
    } else if (cmdLine.hasArg("intel")) {
        m_pciId = BGFX_PCI_ID_INTEL;
    } else if (cmdLine.hasArg("sw")) {
        m_pciId = BGFX_PCI_ID_SOFTWARE_RASTERIZER;
    }
}
