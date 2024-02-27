#include "VertexArray.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray()
{
    GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& buffer, const VertexBufferLayout& layout)
{
    Bind();
    buffer.Bind();

    const auto& elements = layout.GetElements();
    unsigned int offset = 0;

    for(unsigned int i = 0; i < static_cast<unsigned int>(elements.size()); i++)
    {
        const auto& element = elements[i];
        
        GLCall(glEnableVertexAttribArray(i)); 
        GLCall(glVertexAttribPointer(i, element.Count, element.Type, element.IsNormalized, layout.GetStride(), (const void*)offset));

        offset += element.Count * VertexBufferElement::GetSizeOfType(element.Type);
    }
}

void VertexArray::Bind() const
{
    GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
    GLCall(glBindVertexArray(0));
}
