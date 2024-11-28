#include <common.hpp>
#include "engine/engine.hpp"
#include "renderer/cube.hpp"

#ifdef RBX_PREVIEWER_PLATFORM_WINDOWS

#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const auto engine_instance = std::make_unique<engine>();

    engine_instance->init(__argc, __argv);

    auto cube_obj = std::make_shared<cube>();
    engine_instance->add_drawable(cube_obj);

    while (engine_instance->run()) {
    }

    engine_instance->shutdown();

    return 0;
}

#else

int main(int argc, char **argv) {
    auto engine_instance = std::make_unique<engine>();

    engine_instance->init(argc, argv);

    auto cube_obj = std::make_shared<cube>();
    engine_instance->add_drawable(cube_obj);

    while (engine_instance->run()) {
    }

    engine_instance->shutdown();

    return 0;
}

#endif
