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
    uint m_Size;
    uint m_Count;
    unsigned char m_Normalized;
    unsigned long long m_Offset;
    
    VertexBufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
        : m_Name(name), m_Type(type), 
          m_Size(GetSizeOfShaderDataType(type)),
          m_Count(GetComponentCountOfShaderDataType(type)),
          m_Normalized(normalized), m_Offset(0)
    {
    }
    
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
    
    uint GetComponentCountOfShaderDataType(ShaderDataType type) const
    {
        switch (type)
        {
            case ShaderDataType::Char:    return 1;
            case ShaderDataType::Float:   return 1;
            case ShaderDataType::Float2:  return 2;
            case ShaderDataType::Float3:  return 3;
            case ShaderDataType::Float4:  return 4;
            case ShaderDataType::Mat3:    return 3;
            case ShaderDataType::Mat4:    return 4;
            case ShaderDataType::Int:     return 1;
            case ShaderDataType::Int2:    return 2;
            case ShaderDataType::Int3:    return 3;
            case ShaderDataType::Int4:    return 4;
            case ShaderDataType::Bool:    return 1;
        }

        ASSERT(false);
        return 0;
    }
};

class VertexBufferLayout
{
public:
    
    VertexBufferLayout(const std::initializer_list<VertexBufferElement>& elements) 
        : m_VertexBufferElements(elements), m_Stride(0)
    {
        CalculateOffsetsAndStride();
    }

    VertexBufferLayout() : m_Stride(0)
    {
    }
    ~VertexBufferLayout() {}
    
    inline void Push(ShaderDataType type, const std::string& name, bool normalized = false)
    {
        VertexBufferElement element(type, name, normalized);
        m_VertexBufferElements.push_back(element);
        m_Stride += element.m_Size;
    }

    inline const std::vector<VertexBufferElement>& GetElements() const 
    {
        return m_VertexBufferElements;
    }
    
    inline uint GetStride() 
    {
        return m_Stride;
    }
    
private:

    void CalculateOffsetsAndStride()
    {
        uint offset = 0;
        m_Stride = 0;
        for (auto& element : m_VertexBufferElements)
        {
            element.m_Offset = offset;
            offset += element.m_Size;
            m_Stride += element.m_Size;
        }
    }
    
private:

    std::vector<VertexBufferElement> m_VertexBufferElements;
    uint m_Stride;
};
