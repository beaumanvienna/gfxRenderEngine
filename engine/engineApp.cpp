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
#include "renderCommand.h"
#include "core.h"
#include "engineApp.h"
#include "log.h"

bool EngineApp::Start()
{
    //create empty vertex buffer object (vbo)
    vertexBuffer = VertexBuffer::Create();
    
    VertexBufferLayout vertexBufferLayout = 
    {
        {ShaderDataType::Float2, "a_Position"},             // position floats
        {ShaderDataType::Float2, "a_TextureCoordinate"},    // push texture coordinate floats
        {ShaderDataType::Float,  "a_TextureIndex"}          // push texture index float
    };
    vertexBuffer->SetLayout(vertexBufferLayout);
    vertexBuffer->Create(NUMBER_OF_VERTICIES);
    
    //create empty index buffer object (ibo)
    indexBuffer = IndexBuffer::Create();
    
    //create vertex array
    vertexArray = VertexArray::Create();
    vertexArray->AddVertexBuffer(vertexBuffer);
    vertexArray->AddIndexBuffer(indexBuffer);
    
    // program the GPU
    shaderProg = ShaderProgram::Create();
    shaderProg->AddShader(ShaderProgram::VERTEX_SHADER,   "engine/shader/vertexShader.vert");
    shaderProg->AddShader(ShaderProgram::FRAGMENT_SHADER, "engine/shader/fragmentShader.frag");
    shaderProg->Build();
    
    if (!shaderProg->IsOK())
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
    shaderProg->setUniform1iv("u_Textures", 4, textureIDs);
    
    // create Renderer
    renderer = std::make_shared<Renderer>();

    // initializer renderer API
    RenderCommand::EnableBlending();
    RenderCommand::SetClearColor({0.0f, 0.0f, 0.0f, 0.0f});

    // detach everything
    vertexBuffer->Unbind();
    vertexArray->Unbind();
    indexBuffer->Unbind();
    shaderProg->Unbind();

    // camera
    /* projection matrix */
    /* orthographic matrix for projecting two-dimensional coordinates onto the screen */

    /* normalize to -0.5f - 0.5f */
    float normalizeX = 0.5f;
    float normalizeY = 0.5f;

    /* aspect ratio of main window */
    float m_ScaleMainWindowAspectRatio = Engine::m_Engine->GetWindowAspectRatio();

    /* scale it to always have the same physical size on the screen */
    /* independently of the resolution */
    float scaleResolution = 1.0f / Engine::m_Engine->GetWindowScale();

    float ortho_left   =-normalizeX * scaleResolution;                                            
    float ortho_right  = normalizeX * scaleResolution;                                            
    float ortho_bottom =-normalizeY * scaleResolution * m_ScaleMainWindowAspectRatio;             
    float ortho_top    = normalizeY * scaleResolution * m_ScaleMainWindowAspectRatio;             
    float ortho_near   =  1.0f;                                                                   
    float ortho_far    = -1.0f;                                                                   
    
    camera = std::make_shared<OrthographicCamera>(ortho_left, ortho_right, ortho_bottom, ortho_top, ortho_near, ortho_far);

    return true;

}

void EngineApp::Shutdown()
{
}
