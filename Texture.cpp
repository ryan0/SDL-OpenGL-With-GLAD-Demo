//
// Created by ryan on 5/19/2023.
//

#include "Texture.h"
#include "stb_image.h"

Texture::Texture(const std::string& imgPath, bool flippedVertically, GLenum dataFormat) {
    stbi_set_flip_vertically_on_load(flippedVertically);
    unsigned char* imgData = stbi_load(imgPath.c_str(), &imgWidth, &imgHeight, &nrChannels, 0);

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, dataFormat, GL_UNSIGNED_BYTE, imgData);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(imgData);
}

void Texture::bind(GLenum activeTexture) const {
    glActiveTexture(activeTexture);
    glBindTexture(GL_TEXTURE_2D, id);
}
