//
// Created by ryanm on 5/17/2023.
//

#ifndef SDL_OPENGL_WITH_GLAD_DEMO_SHADER_PROGRAM_H
#define SDL_OPENGL_WITH_GLAD_DEMO_SHADER_PROGRAM_H


#include <string>
#include "glad/gl.h"

class ShaderProgram {
public:
    ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);

    void use() const;
    void setUniform(const std::string& name, bool value) const;
    void setUniform(const std::string& name, int value) const;
    void setUniform(const std::string& name, float value) const;

private:
    unsigned int id;

    static std::string loadFileIntoString(const std::string& fileName);
    static unsigned int loadAndCompileShaderFile(const std::string& filename, GLuint glShaderType);
    static unsigned int linkProgramFromShaders(unsigned int vertexShaderID, unsigned int fragmentShaderId);

};


#endif //SDL_OPENGL_WITH_GLAD_DEMO_SHADER_PROGRAM_H
