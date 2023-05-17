#include <glad/gl.h>
#include <SDL.h>
#include <iostream>
#include <fstream>
#include <sstream>

std::string loadFileIntoString(const std::string& fileName) {
    std::ifstream inStream(fileName);
    std::stringstream buffer;
    buffer << inStream.rdbuf();
    return buffer.str();
}

unsigned int loadAndCompileShaderFile(const std::string& filename, GLuint glShaderType) {
    //Load shader str from file, load into shader object and compile
    std::string shaderSource = loadFileIntoString(filename);
    const char* shaderSourceC_str = shaderSource.c_str();
    unsigned int shaderId;
    shaderId = glCreateShader(glShaderType);
    glShaderSource(shaderId, 1, &shaderSourceC_str, nullptr);
    glCompileShader(shaderId);

    //Check success of shader compilation, log error info if failure
    int success;
    char infoLog[512];
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shaderId, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return shaderId;
}

unsigned int linkProgramFromShaders(unsigned int vertexShaderID, unsigned int fragmentShaderId) {
    //Create program and link with vert and frag shaders
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShaderID);
    glAttachShader(shaderProgram, fragmentShaderId);
    glLinkProgram(shaderProgram);

    //Check success of program linking
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "ERROR::PROGRAM::LINKAGE_FAILED\n" << infoLog << std::endl;
    }

    return shaderProgram;
}

struct VertexArray {
    unsigned int vertexArrayId;
    unsigned int arrayBufferId;
    unsigned int elementBufferId;
};

VertexArray createVertexArray(float vertices[], int verticesSize, unsigned int indices[], int indicesSize) {
    VertexArray vao{};

    glGenVertexArrays(1, &vao.vertexArrayId);
    glBindVertexArray(vao.vertexArrayId);

    glGenBuffers(1, &vao.elementBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao.elementBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

    glGenBuffers(1, &vao.arrayBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, vao.arrayBufferId);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    return vao;
}

unsigned int linkProgramFromShaderFiles(const std::string& vertShaderFile, const std::string& fragShaderFile) {
    unsigned int vertexShader = loadAndCompileShaderFile(vertShaderFile, GL_VERTEX_SHADER);
    unsigned int fragmentShader = loadAndCompileShaderFile(fragShaderFile, GL_FRAGMENT_SHADER);
    unsigned int shaderProgram = linkProgramFromShaders(vertexShader, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}

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

    unsigned int shaderProgram = linkProgramFromShaderFiles("../shaders/vert.glsl", "../shaders/frag.glsl");
    unsigned int shaderProgram2 = linkProgramFromShaderFiles("../shaders/vert.glsl", "../shaders/frag-grad.glsl");

    //Define vertices and load into created array buffer object
    float vertices[] = {
            0.5f,  0.5f, 0.0f,  // top right
            0.5f, -0.5f, 0.0f,  // bottom right
            0.2f, -0.5f, 0.0f,  // bottom left
            0.2f,  0.5f, 0.0f   // top left
    };
    unsigned int indices[] = {
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
    };
    VertexArray vao = createVertexArray(vertices, sizeof(vertices), indices, sizeof(indices));

    float vertices2[] = {
            -0.5f,  0.5f, 0.0f,  // top right
            -0.5f, -0.5f, 0.0f,  // bottom right
            -0.2f, -0.5f, 0.0f,  // bottom left
            -0.2f,  0.5f, 0.0f   // top left
    };
    unsigned int indices2[] = {
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
    };
    VertexArray vao2 = createVertexArray(vertices2, sizeof(vertices2), indices2, sizeof(indices2));

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

        glUseProgram(shaderProgram);
        glBindVertexArray(vao.vertexArrayId);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glUseProgram(shaderProgram2);
        glBindVertexArray(vao2.vertexArrayId);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        SDL_GL_SwapWindow(window);
        SDL_Delay(10);
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
