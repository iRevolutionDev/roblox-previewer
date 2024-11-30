
#pragma once

#include <string>
#include <bgfx/bgfx.h>

bgfx::ShaderHandle create_shader(const std::string &shader_path, const char *name);