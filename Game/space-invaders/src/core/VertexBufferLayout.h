#pragma once

#include "Core.h"

#include <vector>

struct VertexBufferElement
{
    unsigned int Type;
    unsigned int Count;
    unsigned char IsNormalized;

    static unsigned int GetSizeOfType(unsigned int type)
    {
        switch (type)
        {
            case GL_FLOAT:          return 4;
            case GL_UNSIGNED_INT:   return 4;
            case GL_UNSIGNED_BYTE:  return 1;
        }

        ASSERT(false);
        return 0;
    }
};

class VertexBufferLayout
{
public:

    template <typename T>
    void Push(unsigned int count)
    { }

    template<>
    void Push<float>(unsigned int count)
    {
        m_Elements.emplace_back(VertexBufferElement{GL_FLOAT, count, GL_FALSE});
        m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
    }

    template<>
    void Push<unsigned int>(unsigned int count)
    {
        m_Elements.emplace_back(VertexBufferElement{GL_UNSIGNED_INT, count, GL_FALSE});
        m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
    }

    template<>
    void Push<unsigned char>(unsigned int count)
    {
        m_Elements.emplace_back(VertexBufferElement{GL_UNSIGNED_BYTE, count, GL_TRUE});
        m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
    }

    inline unsigned int GetStride() const { return m_Stride; }
    inline const std::vector<VertexBufferElement>& GetElements() const {return m_Elements; }

private:

    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride{0};
};
