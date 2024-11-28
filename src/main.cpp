#include <common.hpp>

#include "engine/engine.hpp"

#ifdef RBX_PREVIEWER_PLATFORM_WINDOWS

#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const auto engine_instance = std::make_unique<engine>();

    engine_instance->init(__argc, __argv);

    return 0;
}

#else

int main(int argc, char **argv) {
    auto engine_instance = std::make_unique<engine>();

    engine_instance->init(argc, argv);

    return 0;
}

#endif
