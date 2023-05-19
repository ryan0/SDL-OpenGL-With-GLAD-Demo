//
// Created by ryanm on 5/17/2023.
//

#include <fstream>
#include <sstream>
#include <iostream>
#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(const std::string &vertexPath, const std::string &fragmentPath) {
    unsigned int vertexShader = loadAndCompileShaderFile(vertexPath, GL_VERTEX_SHADER);
    unsigned int fragmentShader = loadAndCompileShaderFile(fragmentPath, GL_FRAGMENT_SHADER);
    unsigned int shaderProgram = linkProgramFromShaders(vertexShader, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    id = shaderProgram;
}

void ShaderProgram::use() const {
    glUseProgram(id);
}

void ShaderProgram::setUniform(const std::string &name, bool value) const {
    use();
    glUniform1i(glGetUniformLocation(id, name.c_str()), int(value));
}

void ShaderProgram::setUniform(const std::string &name, int value) const {
    use();
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void ShaderProgram::setUniform(const std::string &name, float value) const {
    use();
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

std::string ShaderProgram::loadFileIntoString(const std::string &fileName) {
    std::ifstream inStream(fileName);
    std::stringstream buffer;
    buffer << inStream.rdbuf();
    return buffer.str();
}

unsigned int ShaderProgram::loadAndCompileShaderFile(const std::string &filename, GLuint glShaderType) {
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

unsigned int ShaderProgram::linkProgramFromShaders(unsigned int vertexShaderID, unsigned int fragmentShaderId) {
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
