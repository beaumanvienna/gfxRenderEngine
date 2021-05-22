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

#include "engine.h"
#include "platform.h"

#include "vertexArray.h"
#include "buffer.h"
#include "glm.hpp"

class Renderer
{
public:

    enum RendererAPI
    {
        RENDER_API_OPENGL,
        RENDER_API_VULKAN
    };

    virtual bool Create(void* window) = 0;
    virtual void SetClearColor(const glm::vec4& color) = 0;
    virtual void Clear() const = 0;
    virtual void EnableBlending() const = 0;
    virtual void DisableBlending() const = 0;

    // a draw call requires a vertex array (with a vertex buffer bound to it), index buffer, and bound shaders
    virtual void Submit(const VertexArray& vertexArray) const = 0;
    
    static void SetAPI(RendererAPI api) { m_API = api;}
    static RendererAPI GetAPI() { return m_API;}
    static RendererAPI m_API;
    
    virtual void BeginScene();
    virtual void EndScene();

private: 

    
    
};
