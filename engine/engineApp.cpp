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
#include "renderCommand.h"
#include "core.h"
#include "engineApp.h"
#include "log.h"
#include "resources.h"

bool EngineApp::Start()
{
    //create empty vertex buffer object (vbo)
    m_VertexBuffer = VertexBuffer::Create();

    VertexBufferLayout vertexBufferLayout =
    {
        {ShaderDataType::Float3, "a_Position"},             // position floats
        {ShaderDataType::Float2, "a_TextureCoordinate"},    // push texture coordinate floats
        {ShaderDataType::Int,    "a_TextureIndex"},         // push texture index
        {ShaderDataType::Float4, "a_Color"}                 // position floats
    };
    m_VertexBuffer->SetLayout(vertexBufferLayout);
    m_VertexBuffer->Create(NUMBER_OF_VERTICIES);

    //create empty index buffer object (ibo)
    m_IndexBuffer = IndexBuffer::Create();

    //create vertex array
    m_VertexArray = VertexArray::Create();
    m_VertexArray->AddVertexBuffer(m_VertexBuffer);
    m_VertexArray->AddIndexBuffer(m_IndexBuffer);

    // program the GPU
    m_ShaderProg = ShaderProgram::Create();
    m_ShaderProg->AddShader(ShaderProgram::VERTEX_SHADER, "/text/../engine/shader/vertexShader.vert", IDR_VERTEX_SHADER, "TEXT");
    m_ShaderProg->AddShader(ShaderProgram::FRAGMENT_SHADER, "/text/../engine/shader/fragmentShader.frag", IDR_FRAGMENT_SHADER, "TEXT");
    m_ShaderProg->Build();

    if (!m_ShaderProg->IsOK())
    {
        std::cout << "Shader creation failed" << std::endl;
        return false;
    }

    const uint TEXTURE_SLOT_0 = 0;
    int textureIDs[32] =
    {
        TEXTURE_SLOT_0 +  0, TEXTURE_SLOT_0 +  1, TEXTURE_SLOT_0 +  2, TEXTURE_SLOT_0 +  3,
        TEXTURE_SLOT_0 +  4, TEXTURE_SLOT_0 +  5, TEXTURE_SLOT_0 +  6, TEXTURE_SLOT_0 +  7,
        TEXTURE_SLOT_0 +  8, TEXTURE_SLOT_0 +  9, TEXTURE_SLOT_0 + 10, TEXTURE_SLOT_0 + 11,
        TEXTURE_SLOT_0 + 12, TEXTURE_SLOT_0 + 13, TEXTURE_SLOT_0 + 14, TEXTURE_SLOT_0 + 15,
        TEXTURE_SLOT_0 + 16, TEXTURE_SLOT_0 + 17, TEXTURE_SLOT_0 + 18, TEXTURE_SLOT_0 + 19,
        TEXTURE_SLOT_0 + 20, TEXTURE_SLOT_0 + 21, TEXTURE_SLOT_0 + 22, TEXTURE_SLOT_0 + 23,
        TEXTURE_SLOT_0 + 24, TEXTURE_SLOT_0 + 25, TEXTURE_SLOT_0 + 26, TEXTURE_SLOT_0 + 27,
        TEXTURE_SLOT_0 + 28, TEXTURE_SLOT_0 + 29, TEXTURE_SLOT_0 + 30, TEXTURE_SLOT_0 + 31
    };
    m_ShaderProg->SetUniform1iv("u_Textures", 32, textureIDs);

    // create Renderer
    m_Renderer = std::make_shared<Renderer>();
    Engine::m_Engine->SetRenderer(m_Renderer);

    // initializer renderer API
    RenderCommand::EnableBlending();
    RenderCommand::SetClearColor({0.0f, 0.0f, 0.0f, 0.0f});
    RenderCommand::DisableDethTesting();
    RenderCommand::EnableScissor();
    RenderCommand::SetScissor(0, 0, Engine::m_Engine->GetWindowWidth(), Engine::m_Engine->GetWindowHeight());

    // detach everything
    m_VertexBuffer->Unbind();
    m_VertexArray->Unbind();
    m_IndexBuffer->Unbind();
    m_ShaderProg->Unbind();

    // camera
    m_Camera = std::make_shared<OrthographicCamera>();
    m_CameraController = std::make_shared<OrthographicCameraController>(m_Camera);

    return true;

}

void EngineApp::Shutdown()
{
    Engine::m_Engine->Shutdown();
}
