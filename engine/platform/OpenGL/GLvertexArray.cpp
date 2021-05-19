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

#include "GLvertexArray.h"
#include "GL.h"

GLVertexArray::GLVertexArray()
{
    GLCall(glGenVertexArrays(1, &m_RendererID));
}

GLVertexArray::~GLVertexArray()
{
    GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void GLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer, VertexBufferLayout& bufferLayout)
{
    Bind();
    vertexBuffer->Bind();
    auto elements = bufferLayout.GetElements();
    uint i = 0;
    for (auto element : elements)
    {
        //enable vertex attribute(s)
        GLCall(glEnableVertexAttribArray(i));
        GLCall(glVertexAttribPointer
        (
            i,                       /* index in vao */  /* index of the generic vertex attribute to be modified */
            element.m_Count,                             /* number of components per generic vertex attribute */
            ShaderDataTypeToGL(element.m_Type),          /* data type */
            BooleanToGL(element.m_Normalized),           /* normailzed */
            bufferLayout.GetStride(),                    /* data size per drawing object (consecutive generic vertex attributes) */
            (const void*)element.m_Offset                /* offset in vbo */
        ));
        i++;
    }
    
}

void GLVertexArray::Bind() const
{
     GLCall(glBindVertexArray(m_RendererID));
}

void GLVertexArray::Unbind() const
{
    GLCall(glBindVertexArray(INVALID_ID));
}

GLenum GLVertexArray::ShaderDataTypeToGL(ShaderDataType shaderDataType)
{
    switch(shaderDataType)
    {
        case ShaderDataType::Float:     return GL_FLOAT;
        case ShaderDataType::Float2:    return GL_FLOAT;
        case ShaderDataType::Int:       return GL_UNSIGNED_INT;
        case ShaderDataType::Char:      return GL_UNSIGNED_BYTE;
    }
    
    ASSERT(false);
    return 0;
}

GLboolean GLVertexArray::BooleanToGL(bool booleanValue)
{
    switch(booleanValue)
    {
        case true:        return GL_TRUE;
        case false:       return GL_FALSE;
    }

    ASSERT(false);
    return 0;
}


void GLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
{
}
