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

#pragma once

#include <memory>
#include "engine.h"
#include "platform.h"
#include "glm.hpp"
#include "vertexArray.h"
#include "orthographicCamera.h"
#include "shader.h"
#include "spritesheet.h"

class Renderer
{
public:

    static glm::mat4 normalizedPosition;

    Renderer();
    
    // a draw call requires a vertex array (with a vertex buffer bound to it), index buffer, and bound shaders
    virtual void Submit(const std::shared_ptr<VertexArray>& vertexArray);
        
    virtual void BeginScene(std::shared_ptr<OrthographicCamera>& camera, 
                            std::shared_ptr<ShaderProgram>& shader, 
                            std::shared_ptr<VertexBuffer>& vertexBuffer, 
                            std::shared_ptr<IndexBuffer>& indexBuffer);
    virtual void EndScene();
    
    void Draw(Sprite* sprite, const glm::mat4& position, const float depth = 0.0f, bool flipHorizontally=false);
    void Draw(std::shared_ptr<Texture> texture, const glm::mat4& position, const float depth, const glm::vec4 color = glm::vec4(1.0f));

private:

    std::shared_ptr<IndexBuffer> m_IndexBuffer;
    std::shared_ptr<VertexBuffer> m_VertexBuffer;
    std::shared_ptr<ShaderProgram> m_Shader;
};
