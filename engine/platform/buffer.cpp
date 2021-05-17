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

#include "buffer.h"
#include "GLindexBuffer.h"
#include "GLvertexBuffer.h"

std::shared_ptr<VertexBuffer> VertexBuffer::Create(const WindowType windowType)
{
    std::shared_ptr<VertexBuffer> vertexBuffer;

    switch(windowType)
    {
        case WindowType::OPENGL_WINDOW:
            vertexBuffer = std::make_shared<GLVertexBuffer>();
            break;
        default:
            vertexBuffer = nullptr;
            break;
    }

    return vertexBuffer;
}

std::shared_ptr<IndexBuffer> IndexBuffer::Create(const WindowType windowType)
{
    std::shared_ptr<IndexBuffer> indexBuffer;

    switch(windowType)
    {
        case WindowType::OPENGL_WINDOW:
            indexBuffer = std::make_shared<GLIndexBuffer>();
            break;
        default:
            indexBuffer = nullptr;
            break;
    }

    return indexBuffer;
}

