//
// Created by ryan on 5/18/2023.
//

#include "VertexArray.h"
#include "glad/gl.h"

VertexArray::VertexArray(float arrayData[], int arrayDataSize, int elementData[], int elementDataSize) {
    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

    glGenBuffers(1, &elementBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementDataSize, elementData, GL_STATIC_DRAW);

    glGenBuffers(1, &arrayBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, arrayBufferId);
    glBufferData(GL_ARRAY_BUFFER, arrayDataSize, arrayData, GL_STATIC_DRAW);
}

void VertexArray::attribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *offset) const {
    bind();
    glVertexAttribPointer(index, size, type, normalized, stride, offset);
    glEnableVertexAttribArray(index);
}

void VertexArray::bind() const {
    glBindVertexArray(vertexArrayId);
}

unsigned int VertexArray::getArrayBufferId() const {
    return arrayBufferId;
}

unsigned int VertexArray::getElementBufferId() const {
    return elementBufferId;
}
