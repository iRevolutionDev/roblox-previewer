include(FetchContent)

message("GLFW")
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw
        GIT_TAG 3.3.8
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(glfw)
