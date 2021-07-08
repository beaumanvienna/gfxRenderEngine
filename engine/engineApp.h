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
#include "shader.h"
#include "buffer.h"
#include "renderer.h"
#include "spritesheet.h"
#include "event.h"
#include "orthographicCameraController.h"

class EngineApp
{
    
public:

    ~EngineApp() {}
    
    bool Start();
    void Shutdown();
    virtual void OnUpdate() = 0;
    virtual void OnEvent(Event& event) = 0;

public:
    std::shared_ptr<VertexBuffer> m_VertexBuffer;
    std::shared_ptr<IndexBuffer> m_IndexBuffer;
    std::shared_ptr<OrthographicCamera> m_Camera;
    std::shared_ptr<OrthographicCameraController> m_CameraController;

protected:

    std::shared_ptr<ShaderProgram> m_ShaderProg;
    std::shared_ptr<Renderer> m_Renderer;
    
    //create vertex array object (vao)
    std::shared_ptr<VertexArray> m_VertexArray;
    
private:
    
    const uint NUMBER_OF_VERTICIES = 65536;
    
};
