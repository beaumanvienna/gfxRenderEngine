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

enum class ShaderDataType
{
    Char,
    Float, 
    Float2, 
    Float3, 
    Float4, 
    Mat3, 
    Mat4, 
    Int, 
    Int2, 
    Int3, 
    Int4, 
    Bool
};

struct VertexBufferElement
{
    std::string m_Name;
    ShaderDataType m_Type;
    uint m_Count;
    unsigned char m_Normalized;
    
    static uint GetSizeOfShaderDataType(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Char:   return sizeof(char);
            case ShaderDataType::Float:   return sizeof(float);
            case ShaderDataType::Float2:  return sizeof(float) * 2;
            case ShaderDataType::Float3:  return sizeof(float) * 3;
            case ShaderDataType::Float4:  return sizeof(float) * 4;
            case ShaderDataType::Mat3:    return sizeof(float) * 3 * 3;
            case ShaderDataType::Mat4:    return sizeof(float) * 4 * 4;
            case ShaderDataType::Int:     return sizeof(int);
            case ShaderDataType::Int2:    return sizeof(int) * 2;
            case ShaderDataType::Int3:    return sizeof(int) * 3;
            case ShaderDataType::Int4:    return sizeof(int) * 4;
            case ShaderDataType::Bool:    return sizeof(bool);
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
    void Push(std::string name, uint count)
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
inline void VertexBufferLayout::Push<float>(std::string name, uint count)
{
    m_Stride += VertexBufferElement::GetSizeOfShaderDataType(ShaderDataType::Float) * count;
    vertexBufferElements.push_back({name, ShaderDataType::Float, count, false});
}
template <>
inline void VertexBufferLayout::Push<uint>(std::string name, uint count)
{
    m_Stride += VertexBufferElement::GetSizeOfShaderDataType(ShaderDataType::Int) * count;
    vertexBufferElements.push_back({name, ShaderDataType::Int, count, false});
}
template <>
inline void VertexBufferLayout::Push<unsigned char>(std::string name, uint count)
{
    m_Stride += VertexBufferElement::GetSizeOfShaderDataType(ShaderDataType::Char) * count;
    vertexBufferElements.push_back({name, ShaderDataType::Char, count, true});
}
