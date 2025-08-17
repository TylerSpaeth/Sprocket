#include "IndexBuffer.h"
#include "ThirdParty/glad/glad.h"

namespace Sprocket {

    IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) : m_Count(count) {
        glGenBuffers(1, &m_RendererID);
        Bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
        Unbind();
    }

    IndexBuffer::~IndexBuffer() {
        glDeleteBuffers(1, &m_RendererID);
    }

    void IndexBuffer::Bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    }

    void IndexBuffer::Unbind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

}