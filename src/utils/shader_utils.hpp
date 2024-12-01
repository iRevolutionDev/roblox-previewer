
#pragma once

#include <string>
#include <../../cmake-build-debug-visual-studio/_deps/bgfx-src/bgfx/include/bgfx/bgfx.h>

bgfx::ShaderHandle create_shader(const std::string &shader_path, const char *name);