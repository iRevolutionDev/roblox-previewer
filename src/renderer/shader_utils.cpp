
#include "shader_utils.hpp"
#include <filesystem>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <bgfx/bgfx.h>

bgfx::ShaderHandle create_shader(const std::string &shader_path, const char *name) {
    const auto current_path = std::filesystem::current_path();
    const auto path = current_path / "shaders" / shader_path;

    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open shader file: " + path.string());
    }

    const std::vector shader((std::istreambuf_iterator(file)), std::istreambuf_iterator<char>());

    const auto *mem = bgfx::copy(shader.data(), shader.size());
    const auto handle = createShader(mem);
    setName(handle, name);

    return handle;
}