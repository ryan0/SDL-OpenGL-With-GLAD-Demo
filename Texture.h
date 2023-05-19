//
// Created by ryan on 5/19/2023.
//

#ifndef SDL_OPENGL_WITH_GLAD_DEMO_TEXTURE_H
#define SDL_OPENGL_WITH_GLAD_DEMO_TEXTURE_H

#include "glad/gl.h"
#include <string>

class Texture {
public:
    explicit Texture(const std::string& imgPath, bool flippedVertically = false, GLenum dataFormat = GL_RGB);

    void bind(GLenum activeTexture) const;

private:
    unsigned int id = 0;
    int imgWidth = 0;
    int imgHeight = 0;
    int nrChannels = 0;

};


#endif //SDL_OPENGL_WITH_GLAD_DEMO_TEXTURE_H
