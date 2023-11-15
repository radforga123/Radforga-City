#include "VertexArray.h"

namespace RenderEngine
{
    VertexArray::VertexArray()
    {
        glGenVertexArrays(1, &m_id);
    }
    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &m_id);
    }

    VertexArray &VertexArray::operator=(VertexArray &&veretexBuffer)
    {
        m_id = veretexBuffer.m_id;
        veretexBuffer.m_id = 0;
        return *this;
    }
    VertexArray::VertexArray(VertexArray &&veretexBuffer)
    {
        m_id = veretexBuffer.m_id;
        veretexBuffer.m_id = 0;
    }
    void VertexArray::bind() const
    {
        glBindVertexArray(m_id);
    }
    void VertexArray::unbind() const
    {
        glBindVertexArray(0);
    }
    void VertexArray::addBuffer(const VertexBuffer &vertexBuffer, const VertexBufferLayout &layout)
    {
        bind();
        vertexBuffer.bind();
        const auto layoutElements = layout.getLayoutElements();
        GLbyte *offset = nullptr;
        for (unsigned int i = 0; i < layoutElements.size(); ++i)
        {
            const auto &currentLayoutElements = layoutElements[i];
            GLuint currentAttribIndex = m_bufferCount + i;
            glEnableVertexAttribArray(m_bufferCount);
            glVertexAttribPointer(currentAttribIndex, currentLayoutElements.count, currentLayoutElements.type, currentLayoutElements.normalized, layout.getStride(), nullptr);
            offset += currentLayoutElements.size;
        }
        m_bufferCount += static_cast<unsigned int>(layoutElements.size());
    }

};
