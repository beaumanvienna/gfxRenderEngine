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

void Renderer::Draw(std::shared_ptr<Texture> texture, const glm::mat4& position, const float depth, const glm::vec4& color)
{
    int textureSlot = texture->GetTextureSlot();
    glm::vec4 textureCoordinates(0.0f, 0.0f, 1.0f, 1.0f); // entire texture
    FillVertexBuffer(textureSlot, position, depth, color, textureCoordinates);
}

void Renderer::Draw(std::shared_ptr<Texture> texture, const glm::mat4& position, const glm::vec4 textureCoordinates, const float depth, const glm::vec4& color)
{
    int textureSlot = texture->GetTextureSlot();
    FillVertexBuffer(textureSlot, position, depth, color, textureCoordinates);
}

void Renderer::Draw(Sprite* sprite, const glm::mat4& position, const float depth, const glm::vec4& color)
{
    int textureSlot = sprite->GetTextureSlot();
    glm::vec4 textureCoordinates(sprite->m_Pos1X, sprite->m_Pos1Y, sprite->m_Pos2X, sprite->m_Pos2Y);
    FillVertexBuffer(textureSlot, position, depth, color, textureCoordinates);
}
    
void Renderer::FillVertexBuffer(const int textureSlot, const glm::mat4& position, const float depth, const glm::vec4& color, const glm::vec4& textureCoordinates)
{
    //fill index buffer object (ibo)
    m_IndexBuffer->AddObject(IndexBuffer::INDEX_BUFFER_QUAD);

    float pos1X = textureCoordinates.x; 
    float pos2X = textureCoordinates.z;    
    float pos1Y = textureCoordinates.y;
    float pos2Y = textureCoordinates.w;
    
    float verticies[] = 
    { /*   positions   */ /* texture coordinate */
         position[0][0], position[0][1], depth, pos1X, pos1Y, CastToFloat(textureSlot), color.r, color.g, color.b, color.a,  //    0.0f,  1.0f,
         position[1][0], position[1][1], depth, pos2X, pos1Y, CastToFloat(textureSlot), color.r, color.g, color.b, color.a,  //    1.0f,  1.0f, // position 2
         position[2][0], position[2][1], depth, pos2X, pos2Y, CastToFloat(textureSlot), color.r, color.g, color.b, color.a,  //    1.0f,  0.0f, 
         position[3][0], position[3][1], depth, pos1X, pos2Y, CastToFloat(textureSlot), color.r, color.g, color.b, color.a   //    0.0f,  0.0f  // position 1
    };
    
    m_VertexBuffer->LoadBuffer(verticies, sizeof(verticies));
}

void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
{
    vertexArray->Bind();
    RenderCommand::DrawIndexed(vertexArray);
}

