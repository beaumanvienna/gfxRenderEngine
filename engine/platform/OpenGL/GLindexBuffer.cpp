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

#include "GLindexBuffer.h"
#include "GL.h"

GLIndexBuffer::GLIndexBuffer(const uint* indicies, uint count) : m_VertexCount(0)
{
    GLCall(glGenBuffers(1, &m_RendererID));
    Bind();
    // load data into ibo
    GLCall(glBufferData
    (
        GL_ELEMENT_ARRAY_BUFFER,        /* target */
        count * sizeof(uint),           /* buffer size */
        indicies,                       /* actual data */
        GL_STATIC_DRAW                  /* usage */
    ));
}

GLIndexBuffer::GLIndexBuffer()  : m_VertexCount(0)
{
    GLCall(glGenBuffers(1, &m_RendererID));
    Bind();
    // load data into ibo
    GLCall(glBufferData
    (
        GL_ELEMENT_ARRAY_BUFFER,        /* target */
        0,                              /* buffer size */
        nullptr,                        /* actual data */
        GL_DYNAMIC_DRAW                 /* usage */
    ));
}

GLIndexBuffer::~GLIndexBuffer()
{
    GLCall(glDeleteBuffers(1,&m_RendererID));
}

void GLIndexBuffer::AddObject(IndexBufferObject object)
{
    switch (object)
    {
        case INDEX_BUFFER_QUAD:
        
            //0,1,3,   /*first triangle */
            //
            //1,2,3,   /* second triangle */
            
            m_Indicies.push_back(0 + m_VertexCount);
            m_Indicies.push_back(1 + m_VertexCount);
            m_Indicies.push_back(3 + m_VertexCount);
            m_Indicies.push_back(1 + m_VertexCount);
            m_Indicies.push_back(2 + m_VertexCount);
            m_Indicies.push_back(3 + m_VertexCount);
            
            m_VertexCount += 4; // four new verticies
            
            break;
        default:
            // not found
            break;
    }
}


void GLIndexBuffer::BeginScene()
{
    m_Indicies.clear(); 
    m_VertexCount = 0;
}

void GLIndexBuffer::EndScene()
{
    if (!m_Indicies.size()) return;
    Bind();
    // load data into ibo
    GLCall(glBufferData
    (
        GL_ELEMENT_ARRAY_BUFFER,            /* target */
        m_Indicies.size() * sizeof(uint),   /* buffer size */
        (const void*)&m_Indicies[0],            /* actual data */
        GL_DYNAMIC_DRAW                     /* usage */
    ));
}

void GLIndexBuffer::Bind() const
{
     GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void GLIndexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, INVALID_ID));
}
