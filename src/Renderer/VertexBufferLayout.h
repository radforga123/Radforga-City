#pragma once

#include <vector>
#include <glad/glad.h>

namespace RenderEngine
{
    struct VertexBufferLayoutElements
    {
        GLint count;
        GLenum type;
        GLboolean normalized;
        unsigned int size;
    };

    class VertexBufferLayout
    {
    public:
        VertexBufferLayout(/* args */);

        void reserveElements(const size_t count);
        unsigned int getStride() const { return m_stride; }
        void addElementArray(const unsigned int count, const bool normalized);
        const std::vector<VertexBufferLayoutElements> &getLayoutElements() const { return m_layoutElements; };

    private:
        std::vector<VertexBufferLayoutElements> m_layoutElements;
        unsigned int m_stride;
    };

}
