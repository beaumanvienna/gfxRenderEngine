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
   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
    
   The code in this file is based on and inspired by the project
   https://github.com/TheCherno/Hazel. The license of this prject can
   be found under https://github.com/TheCherno/Hazel/blob/master/LICENSE
   */

#pragma once

#include "window.h"
#include <memory>
#include "vertexBufferLayout.h"

class VertexBuffer
{

public:

    struct Vertex
    {
        float m_Position[2]; // 2D
        float m_TextureCoordinates[2]; 
        float m_Index;
    };

    virtual ~VertexBuffer() {}

    virtual void Create(uint count) = 0;
    virtual void LoadBuffer(const void* verticies, uint size) = 0;
    virtual void BeginScene() = 0;
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
    
    virtual const VertexBufferLayout& GetLayout() const = 0;
    virtual void SetLayout(const VertexBufferLayout& layout) = 0;
    
    static std::shared_ptr<VertexBuffer> Create();

};

class IndexBuffer
{
public:

    enum IndexBufferObject
    {
        INDEX_BUFFER_TRIANGLE,
        INDEX_BUFFER_QUAD
    };

    virtual ~IndexBuffer() {}
    
    virtual void AddObject(IndexBufferObject object) = 0;
    virtual void BeginScene() = 0;
    virtual void EndScene() = 0;
    
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
    virtual uint GetCount() const = 0;
    
    static std::shared_ptr<IndexBuffer> Create();

};

