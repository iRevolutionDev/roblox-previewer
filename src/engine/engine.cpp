#include "common.hpp"

#include "engine.hpp"

#include "utils/utils.hpp"

engine::engine() {
    g_engine = this;
}

engine::~engine() {
    g_engine = nullptr;
}

void engine::init(const int32_t argc, char **argv) const {
    const EngineArgs args(argc, argv);

    bgfx::Init init{};
    init.type = args.m_type;
    init.vendorId = args.m_pciId;
    init.resolution.width = m_width;
    init.resolution.height = m_height;
    init.resolution.reset = m_reset;

    bgfx::init(init);
}

