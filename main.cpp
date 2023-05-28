#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <SDL.h>
#include <iostream>
#include <sstream>
#include "ShaderProgram.h"
#include "VertexArray.h"
#include "Texture.h"

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

    SDL_ShowCursor(SDL_DISABLE);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    //Load OpenGL function pointers using GLAD
    int version = gladLoadGL((GLADloadfunc) SDL_GL_GetProcAddress);
    std::cout<<"Glad loadGL version: "<<version<<std::endl;

    // Check OpenGL properties;
    std::cout<<"OpenGL loaded\n";
    std::cout<<"Vendor:    "<<glGetString(GL_VENDOR)<<std::endl;
    std::cout<<"Renderer:  "<<glGetString(GL_RENDERER)<<std::endl;
    std::cout<<"Version:   "<<glGetString(GL_VERSION)<<std::endl;

    glEnable(GL_DEPTH_TEST);

    ShaderProgram shaderProgram = ShaderProgram("../shaders/vert.glsl", "../shaders/frag.glsl");

    //Define vertices and load into created array buffer object
    float vertices[] = {
             -0.5f, -0.5f, 0.5f,     0.0f, 0.0f, //Front Vertices
             -0.5f,  0.5f, 0.5f,     0.0f, 1.0f,
              0.5f,  0.5f, 0.5f,     1.0f, 1.0f,
              0.5f, -0.5f, 0.5f,     1.0f, 0.0f,

             -0.5f, -0.5f, -0.5f,     0.0f, 0.0f, //Back Vertices
             -0.5f,  0.5f, -0.5f,     0.0f, 1.0f,
              0.5f,  0.5f, -0.5f,     1.0f, 1.0f,
              0.5f, -0.5f, -0.5f,     1.0f, 0.0f,

             -0.5f, -0.5f, 0.5f,     0.0f, 0.0f, //Bottom Vertices
             -0.5f, -0.5f,-0.5f,     0.0f, 1.0f,
              0.5f, -0.5f,-0.5f,     1.0f, 1.0f,
              0.5f, -0.5f, 0.5f,     1.0f, 0.0f,

             -0.5f,  0.5f, 0.5f,     0.0f, 0.0f, //Top Vertices
             -0.5f,  0.5f,-0.5f,     0.0f, 1.0f,
              0.5f,  0.5f,-0.5f,     1.0f, 1.0f,
              0.5f,  0.5f, 0.5f,     1.0f, 0.0f,

              0.5f,  0.5f, 0.5f,     0.0f, 0.0f, //Right Vertices
              0.5f,  0.5f,-0.5f,     0.0f, 1.0f,
              0.5f, -0.5f,-0.5f,     1.0f, 1.0f,
              0.5f, -0.5f, 0.5f,     1.0f, 0.0f,

             -0.5f,  0.5f, 0.5f,     0.0f, 0.0f, //Left Vertices
             -0.5f,  0.5f,-0.5f,     0.0f, 1.0f,
             -0.5f, -0.5f,-0.5f,     1.0f, 1.0f,
             -0.5f, -0.5f, 0.5f,     1.0f, 0.0f,
    };
    int indices[] = {
            0, 1, 2,    0, 2, 3,   // Front face
            4, 5, 6,    4, 6, 7,    // Back face
            8, 9, 10,    8, 10, 11,    // Bottom face
            12, 13, 14,  12, 14, 15,    // Top face
            16, 17, 18,  16, 18, 19,    //Right Face
            20, 21, 22,  20, 22, 23     //Left Face
    };
    VertexArray vertArray1{vertices, sizeof(vertices), indices, sizeof(indices)};
    vertArray1.attribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    vertArray1.attribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    Texture tex1 = Texture("../assets/container.jpg");


    glm::mat4 model = glm::mat4(1.0f);

    float aspect = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

    glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, -5.0f);

    float xRotation = 0.0f;
    float yRotation = 0.0f;

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event) != 0) {
            if(event.type == SDL_QUIT) {
                running = false;
            }
            if(event.type == SDL_MOUSEMOTION) {
                xRotation += static_cast<float>(event.motion.xrel) / 2.0f;
                yRotation += static_cast<float>(event.motion.yrel) / 2.0f;
            }
        }
        const Uint8* currentKeyStates = SDL_GetKeyboardState(nullptr);
        if(currentKeyStates[SDL_SCANCODE_W]) {
            glm::vec3 offset(0.0f, 0.0f, 0.05f);
            cameraPosition += glm::rotate(offset, glm::radians(-xRotation), glm::vec3(0.0f, 1.0f, 0.0f));
        }
        if(currentKeyStates[SDL_SCANCODE_S]) {
            glm::vec3 offset(0.0f, 0.0f, -0.05f);
            cameraPosition += glm::rotate(offset, glm::radians(-xRotation), glm::vec3(0.0f, 1.0f, 0.0f));
        }
        if(currentKeyStates[SDL_SCANCODE_A]) {
            glm::vec3 offset(0.05f, 0.0f, 0.0f);
            cameraPosition += glm::rotate(offset, glm::radians(-xRotation), glm::vec3(0.0f, 1.0f, 0.0f));
        }
        if(currentKeyStates[SDL_SCANCODE_D]) {
            glm::vec3 offset(-0.05f, 0.0f, 0.0f);
            cameraPosition += glm::rotate(offset, glm::radians(-xRotation), glm::vec3(0.0f, 1.0f, 0.0f));
        }


        glClearColor(0.0f, 0.35f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = glm::mat4(1.0f);
        view = glm::rotate(view, glm::radians(yRotation), glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::rotate(view, glm::radians(xRotation), glm::vec3(0.0f, 1.0f, 0.0f));
        view = glm::translate(view, cameraPosition);

        shaderProgram.use();

        tex1.bind(GL_TEXTURE0);
        shaderProgram.setUniform1i("texture1", 0);

        shaderProgram.setUniformMat4fv("model", model);
        shaderProgram.setUniformMat4fv("view", view);
        shaderProgram.setUniformMat4fv("projection", projection);

        vertArray1.bind();
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);


        SDL_GL_SwapWindow(window);
        SDL_Delay(10);
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
