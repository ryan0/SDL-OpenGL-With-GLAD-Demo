//
// Created by ryan on 5/18/2023.
//

#ifndef SDL_OPENGL_WITH_GLAD_DEMO_VERTEX_ARRAY_H
#define SDL_OPENGL_WITH_GLAD_DEMO_VERTEX_ARRAY_H

#include "glad/gl.h"

class VertexArray {
public:
    VertexArray(float arrayData[], int arrayDataSize, int elementData[], int elementDataSize);

    void attribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* offset) const;

    void bind() const;
    [[nodiscard]] unsigned int getArrayBufferId() const;
    [[nodiscard]] unsigned int getElementBufferId() const;

private:
    unsigned int vertexArrayId = 0;
    unsigned int arrayBufferId = 0;
    unsigned int elementBufferId = 0;
};


#endif //SDL_OPENGL_WITH_GLAD_DEMO_VERTEX_ARRAY_H
