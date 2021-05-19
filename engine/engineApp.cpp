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

#include "engine.h"
#include "platform.h"
#include "core.h"
#include "engineApp.h"
#include "log.h"

EngineApp::EngineApp()
{
}

EngineApp::~EngineApp()
{
}

bool EngineApp::Start()
{
    //create empty vertex buffer object (vbo)
    vertexBuffer = VertexBuffer::Create();
    vertexBuffer->Create(sizeof(VertexBuffer::Vertex) * NUMBER_OF_VERTICIES);
    
    VertexBufferLayout vertexBufferLayout = 
    {
        {ShaderDataType::Float2, "a_Position"},             // position floats
        {ShaderDataType::Float2, "a_TextureCoordinate"},    // push texture coordinate floats
        {ShaderDataType::Float,  "a_TextureIndex"}          // push texture index float
    };
    
    vertexArray = VertexArray::Create();
    vertexArray->AddVertexBuffer(vertexBuffer, vertexBufferLayout);
    
    //create empty index buffer object (ibo)
    indexBuffer = IndexBuffer::Create();
    
    // program the GPU
    shaderProg.AddShader(GL_VERTEX_SHADER,   "engine/shader/vertexShader.vert");
    shaderProg.AddShader(GL_FRAGMENT_SHADER, "engine/shader/fragmentShader.frag");
    shaderProg.Create();
    
    if (!shaderProg.IsOK())
    {
        std::cout << "Shader creation failed" << std::endl;
        return false;
    }
    
    const uint TEXTURE_SLOT_0 = 0;
    int textureIDs[8] = 
    {
        TEXTURE_SLOT_0 + 0, TEXTURE_SLOT_0 + 1, TEXTURE_SLOT_0 + 2, TEXTURE_SLOT_0 + 3,
        TEXTURE_SLOT_0 + 4, TEXTURE_SLOT_0 + 5, TEXTURE_SLOT_0 + 6, TEXTURE_SLOT_0 + 7
    };
    shaderProg.setUniform1iv("u_Textures", 4, textureIDs);
    
    // create Renderer
    renderer.Create((GLFWwindow*)Engine::m_Engine->GetWindow());
    renderer.EnableBlending();

    // detach everything
    vertexBuffer->Unbind();
    vertexArray->Unbind();
    indexBuffer->Unbind();
    shaderProg.Unbind();

    return true;

}

void EngineApp::Shutdown()
{
}
