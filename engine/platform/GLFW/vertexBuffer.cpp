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

#include "vertexBuffer.h"
#include "GLFW/GL.h"


VertexBuffer::VertexBuffer()
{
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1,&m_RendererID));
}

void VertexBuffer::Create(uint size)
{
    GLCall(glGenBuffers(1, &m_RendererID));
    Bind();
    // load data into vbo
    GLCall(glBufferData
    (
        GL_ARRAY_BUFFER,                /* target */
        size,                           /* buffer size */
        nullptr,                        /* empty for now */
        GL_DYNAMIC_DRAW                 /* usage */
    ));
    uint m_BuferOffset = 0;
}

void VertexBuffer::LoadBuffer(const void* verticies, uint size)
{
    Bind();
    // load data into vbo
    GLCall(glBufferSubData
    (
        GL_ARRAY_BUFFER,                /* target */
        m_BufferOffset,                 /* offset */
        size,                           /* buffer size */
        (const void*)verticies          /* actual data */
    ));
    
    m_BufferOffset += size;
}

void VertexBuffer::Bind() const
{
     GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, INVALID_ID));
}