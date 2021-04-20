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

#include "vertexArray.h"
#include "OpenGL/GL.h"


VertexArray::VertexArray()
{
    GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vertexBuffer, VertexBufferLayout& bufferLayout)
{
    Bind();
    vertexBuffer.Bind();
    auto elements = bufferLayout.GetElements();
    uint i = 0;
    unsigned long long offset = 0;
    for (auto element : elements)
    {
        //enable vertex attribute(s)
        GLCall(glEnableVertexAttribArray(i));
        GLCall(glVertexAttribPointer
        (
            i,                       /* index in vao */  /* index of the generic vertex attribute to be modified */
            element.count,                               /* number of components per generic vertex attribute */
            element.type,                                /* data type */
            element.normalized,                          /* normailzed */
            bufferLayout.GetStride(),                    /* data size per drawing object (consecutive generic vertex attributes) */
            (const void*)offset                          /* offset in vbo */
        ));
        i++;
        offset += element.count*VertexBufferElement::getSizeOfType(element.type);
    }
    
}

void VertexArray::Bind() const
{
     GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
    GLCall(glBindVertexArray(INVALID_ID));
}
