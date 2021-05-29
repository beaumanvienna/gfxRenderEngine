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

#include "splash.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

void Splash::OnAttach() 
{ 
    m_SpritesheetSplash.AddSpritesheetAnimation("resources/splashscreen/splash_spritesheet2.png", 20, 200);
    m_Splash = m_SpritesheetSplash.GetSpriteAnimation();
    m_Splash->Start();
}

void Splash::OnDetach() 
{
    
}

void Splash::OnUpdate() 
{
    if (m_Splash->IsRunning()) 
    {
        static bool splashMessage = true;
        if (splashMessage)
        {
            splashMessage = false;
            LOG_APP_INFO("splash is running");
        }

        m_SpritesheetSplash.BeginScene();
        {
            //fill index buffer object (ibo)
            m_IndexBuffer->AddObject(IndexBuffer::INDEX_BUFFER_QUAD);
        
            sprite = m_Splash->GetSprite();
                        
            glm::mat4  modelMatrix = sprite->GetScale() * glm::mat4(1.0f);

            // --- combine model and camera matrixes into MVP matrix---
            glm::mat4 model_view_projection = modelMatrix * m_Camera->GetViewProjectionMatrix();
            
            // --- load into vertex buffer ---
            glm::mat4 normalizedPosition  = glm::mat4
            (
                -0.5f,  0.5f, 1.0f, 1.0f,
                 0.5f,  0.5f, 1.0f, 1.0f,
                 0.5f, -0.5f, 1.0f, 1.0f,
                -0.5f, -0.5f, 1.0f, 1.0f
            );
            glm::mat4 position  = model_view_projection * normalizedPosition;

            float pos1X = sprite->m_Pos1X; 
            float pos1Y = sprite->m_Pos1Y; 
            float pos2X = sprite->m_Pos2X;
            float pos2Y = sprite->m_Pos2Y;

            float textureID = static_cast<float>(m_SpritesheetSplash.GetTextureSlot());

            float verticies[] = 
            { /*   positions   */ /* texture coordinate */
                 position[0][0], position[0][1], pos1X, pos1Y, textureID, //    0.0f,  1.0f,
                 position[1][0], position[1][1], pos2X, pos1Y, textureID, //    1.0f,  1.0f, // position 2
                 position[2][0], position[2][1], pos2X, pos2Y, textureID, //    1.0f,  0.0f, 
                 position[3][0], position[3][1], pos1X, pos2Y, textureID  //    0.0f,  0.0f  // position 1
            };
            
            m_VertexBuffer->LoadBuffer(verticies, sizeof(verticies));
        }
        
    }
}

void Splash::OnEvent(Event& event) 
{
}
