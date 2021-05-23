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

#include <memory>

#include "core.h"
#include "log.h"
#include "GLshader.h"
#include "buffer.h"
#include "renderer.h"
#include "GLtexture.h"
#include "spritesheet.h"
#include "event.h"

class EngineApp
{
    
public:

    EngineApp();
    virtual ~EngineApp();
    
    bool Start();
    void Shutdown();
    virtual void OnUpdate() = 0;
    virtual void OnEvent(Event& event) = 0;
    
    std::shared_ptr<VertexBuffer> vertexBuffer;
    std::shared_ptr<IndexBuffer> indexBuffer;

protected:

    ShaderProgram shaderProg;
    std::shared_ptr<Renderer> renderer;
    
    //create vertex array object (vao)
    std::shared_ptr<VertexArray> vertexArray;
    
private:
    
    const uint NUMBER_OF_VERTICIES = 1024;
    
};
