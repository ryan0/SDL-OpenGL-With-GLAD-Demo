#include <glad/gl.h>
#include <SDL.h>
#include <iostream>

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_LoadLibrary(nullptr);

    // Request an OpenGL 4.5 context (should be core)
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);

    const int SCREEN_WIDTH = 1920;
    const int SCREEN_HEIGHT = 1080;

    SDL_Window* window;
    window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL );
    SDL_GLContext context = SDL_GL_CreateContext(window);

    int version = gladLoadGL((GLADloadfunc) SDL_GL_GetProcAddress);
    std::cout<<"Glad loadGL version: "<<version<<std::endl;

    // Check OpenGL properties;
    std::cout<<"OpenGL loaded\n";
    std::cout<<"Vendor:    "<<glGetString(GL_VENDOR)<<std::endl;
    std::cout<<"Renderer:  "<<glGetString(GL_RENDERER)<<std::endl;
    std::cout<<"Version:   "<<glGetString(GL_VERSION)<<std::endl;

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

        SDL_GL_SwapWindow(window);
        SDL_Delay(10);
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
