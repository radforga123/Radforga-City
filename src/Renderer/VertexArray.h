#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

#include <glad/glad.h>

namespace RenderEngine
{
    class VertexArray
    {
    public:
        VertexArray(/* args */);
        ~VertexArray();

        VertexArray(const VertexArray &) = delete;
        VertexArray &operator=(const VertexArray &) = delete;
        VertexArray &operator=(VertexArray &&veretexArray);
        VertexArray(VertexArray &&veretexArray);

        void addBuffer(const VertexBuffer &vertexBuffer, const VertexBufferLayout &layout);
        void bind() const;
        void unbind() const;

    private:
        GLuint m_id = 0;
        size_t m_bufferCount = 0;
    };

}