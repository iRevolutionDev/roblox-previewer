cmake_minimum_required(VERSION 3.29)
project(roblox_previewer VERSION 0.1.0 DESCRIPTION "A simple Roblox model previewer")

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(TEST_DIR "${PROJECT_SOURCE_DIR}/test")

# Scripts Packages
message(STATUS "Feching packages...")
include(scripts/bgfx.cmake)
include(scripts/glfw.cmake)

# Project Files
file(GLOB_RECURSE SRC_FILES
        "${SRC_DIR}/**.cpp"
        "${SRC_DIR}/**.hpp"
        "${SRC_DIR}/**.h"
)

if (WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${SRC_FILES})
else ()
    add_executable(${PROJECT_NAME} ${SRC_FILES})
endif ()

# Compiler Options
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 26)

source_group(TREE "${SRC_DIR}" PREFIX "src" FILES ${SRC_FILES})

target_precompile_headers(${PROJECT_NAME} PRIVATE "${SRC_DIR}/common.hpp")

target_include_directories(${PROJECT_NAME} PUBLIC
        "${SRC_DIR}"
        "${BGFX_INCLUDE_DIR}"
        "${BIMG_INCLUDE_DIR}"
        "${BX_INCLUDE_DIR}"
        "${GLFW_INCLUDE_DIR}"
)

target_link_libraries(${PROJECT_NAME} PUBLIC
        bgfx
        bimg
        bx
        glfw
        ${CMAKE_DL_LIBS}
)

if (WIN32)
    target_compile_definitions(${PROJECT_NAME} PUBLIC RBX_PREVIEWER_PLATFORM_WINDOWS)
elseif (UNIX)
    target_compile_definitions(${PROJECT_NAME} PUBLIC RBX_PREVIEWER_PLATFORM_LINUX)
endif ()

