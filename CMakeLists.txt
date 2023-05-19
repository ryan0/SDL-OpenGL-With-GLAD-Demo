cmake_minimum_required(VERSION 3.25)
project(SDL_Opengl_With_Glad_Demo)

set(CMAKE_CXX_STANDARD 17)

include_directories(${PROJECT_SOURCE_DIR}/glad/include)

add_executable(SDL_Opengl_With_Glad_Demo main.cpp glad/src/gl.c ShaderProgram.cpp ShaderProgram.h VertexArray.cpp VertexArray.h stb_image.h Texture.cpp Texture.h)

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(
        SDL_Opengl_With_Glad_Demo PUBLIC
        SDL2::SDL2main
        SDL2::SDL2
)
