/* Engine Copyright (c) 2021 Engine Development Team 
   https://github.com/beaumanvienna/gfxRenderEngine

   Permission is hereby granted, free of charge, to any person
   obtaining a copy of this software and associated documentation files
   (the "Software"), to deal in the Software without restriction,
   including without limitation the rights to use, copy, modify, merge,
   publish, distribute, sublicense, and/or sell copies of the Software,
   and to permit persons to whom the Software is furnished to do so,
   subject to the following conditions:

   The above copyright notice and this permission notice shall be
   included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
   IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY 
   CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
   TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */

#pragma once

#include <vector>

#include "engine.h"
#include "platform.h"
#include "GLFW/GL.h"
#include "vertexBuffer.h"

struct VertexBufferElement
{
    uint type;
    uint count;
    unsigned char normalized;
    
    static uint getSizeOfType(uint type)
    {
        switch(type)
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
    VertexBufferLayout() : m_Stride(0)
    {
    }
    ~VertexBufferLayout() {}
    
    template <typename T>
    void Push(uint count)
    {
        ASSERT(false);
    }
    
    inline const std::vector<VertexBufferElement>& GetElements() const 
    {
        return vertexBufferElements;
    }
    
    inline uint GetStride() 
    {
        return m_Stride;
    }
    
private:
    std::vector<VertexBufferElement> vertexBufferElements;
    uint m_Stride;
};

template <>
inline void VertexBufferLayout::Push<float>(uint count)
{
    m_Stride += VertexBufferElement::getSizeOfType(GL_FLOAT) * count;
    vertexBufferElements.push_back({GL_FLOAT,count, GL_FALSE});
}
template <>
inline void VertexBufferLayout::Push<uint>(uint count)
{
    m_Stride += VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT) * count;
    vertexBufferElements.push_back({GL_UNSIGNED_INT,count, GL_FALSE});
}
template <>
inline void VertexBufferLayout::Push<unsigned char>(uint count)
{
    m_Stride += VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE) * count;
    vertexBufferElements.push_back({GL_UNSIGNED_BYTE,count, GL_TRUE});
}
