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
   
#include "renderer.h"
#include "rendererAPI.h"
#include "renderCommand.h"

Renderer::Renderer()
{ 
    RendererAPI::Create(); 
}

void Renderer::BeginScene(std::shared_ptr<OrthographicCamera>& camera, 
                            std::shared_ptr<ShaderProgram>& shader, 
                            std::shared_ptr<VertexBuffer>& vertexBuffer, 
                            std::shared_ptr<IndexBuffer>& indexBuffer)
{
    m_IndexBuffer = indexBuffer;
    m_VertexBuffer = vertexBuffer;
    m_Shader = shader;
    
    m_Shader->Bind();
    m_VertexBuffer->BeginScene();
    m_IndexBuffer->BeginScene();
    
    m_Shader->SetUniformMat4f("u_ViewProjectionMatrix", camera->GetViewProjectionMatrix());
}

void Renderer::EndScene()
{
}

void Renderer::Draw(Sprite* sprite, const glm::mat4& position, const int textureSlot, bool flipHorizontally)
{
    //fill index buffer object (ibo)
    m_IndexBuffer->AddObject(IndexBuffer::INDEX_BUFFER_QUAD);

    float pos1X; 
    float pos2X; 

    if (flipHorizontally)
    {
        pos2X = sprite->m_Pos1X; 
        pos1X = sprite->m_Pos2X;
    }
    else
    {
        pos1X = sprite->m_Pos1X; 
        pos2X = sprite->m_Pos2X;
    }
    float pos1Y = sprite->m_Pos1Y; 
    float pos2Y = sprite->m_Pos2Y;
    
    float verticies[] = 
    { /*   positions   */ /* texture coordinate */
         position[0][0], position[0][1], pos1X, pos1Y, ((float*)(&textureSlot))[0],  //    0.0f,  1.0f,
         position[1][0], position[1][1], pos2X, pos1Y, ((float*)(&textureSlot))[0],  //    1.0f,  1.0f, // position 2
         position[2][0], position[2][1], pos2X, pos2Y, ((float*)(&textureSlot))[0],  //    1.0f,  0.0f, 
         position[3][0], position[3][1], pos1X, pos2Y, ((float*)(&textureSlot))[0]   //    0.0f,  0.0f  // position 1
    };
    
    m_VertexBuffer->LoadBuffer(verticies, sizeof(verticies));
}

void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
{
    vertexArray->Bind();
    RenderCommand::DrawIndexed(vertexArray);
}

