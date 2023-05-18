#include <glad/gl.h>
#include <SDL.h>
#include <iostream>
#include <sstream>
#include "ShaderProgram.h"
#include "VertexArray.h"

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_LoadLibrary(nullptr);

    // Request an OpenGL 4.5 context (should be core)
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);

    //Define screen dimensions, create SDL window and associated OpenGL context
    const int SCREEN_WIDTH = 1920;
    const int SCREEN_HEIGHT = 1080;
    SDL_Window* window;
    window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL );
    SDL_GLContext context = SDL_GL_CreateContext(window);

    //Load OpenGL function pointers using GLAD
    int version = gladLoadGL((GLADloadfunc) SDL_GL_GetProcAddress);
    std::cout<<"Glad loadGL version: "<<version<<std::endl;

    // Check OpenGL properties;
    std::cout<<"OpenGL loaded\n";
    std::cout<<"Vendor:    "<<glGetString(GL_VENDOR)<<std::endl;
    std::cout<<"Renderer:  "<<glGetString(GL_RENDERER)<<std::endl;
    std::cout<<"Version:   "<<glGetString(GL_VERSION)<<std::endl;

    ShaderProgram shaderProgram = ShaderProgram("../shaders/vert.glsl", "../shaders/frag.glsl");
    ShaderProgram shaderProgram2 = ShaderProgram("../shaders/vert2.glsl", "../shaders/frag-grad.glsl");

    //Define vertices and load into created array buffer object
    float vertices[] = {
            0.5f, 0.5f, 0.0f,  // top right
            0.5f, -0.5f, 0.0f,  // bottom right
            0.2f, -0.5f, 0.0f,  // bottom left
            0.2f, 0.5f, 0.0f   // top left
    };
    int indices[] = {
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
    };
    VertexArray vertArray1{vertices, sizeof(vertices), indices, sizeof(indices)};
    vertArray1.attribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    float vertices2[] = {
            -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // top right
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom right
            -0.2f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom left
            -0.2f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f // top left
    };
    int indices2[] = {
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
    };
    VertexArray vertArray{vertices2, sizeof(vertices2), indices2, sizeof(indices2)};
    vertArray.attribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    vertArray.attribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event) != 0) {
            if(event.type == SDL_QUIT) {
                running = false;
            }
        }
        glClearColor(0.0f, 0.35f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.use();
        vertArray1.bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        shaderProgram2.use();
        vertArray.bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        SDL_GL_SwapWindow(window);
        SDL_Delay(10);
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
