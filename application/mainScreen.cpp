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

#include "mainScreen.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

bool showTabIcons = true;

void MainScreenLayer::OnAttach() 
{     
    m_SpritesheetMarley.AddSpritesheetPPSSPP("resources/images/ui_atlas/ui_atlas.png");
    
    m_TranslationSpeedClouds = 0.001f;
    
    normalizedPosition  = glm::mat4
    (
        -0.5f,  0.5f, 1.0f, 1.0f,
         0.5f,  0.5f, 1.0f, 1.0f,
         0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, 1.0f, 1.0f
    );
    
}

void MainScreenLayer::OnDetach() 
{
    
}

void MainScreenLayer::OnUpdate() 
{
    static bool mainMessage = true;
    if (mainMessage)
    {
        mainMessage = false;
        LOG_APP_INFO("main screen is running");
    }
    m_SpritesheetMarley.BeginScene();
    
    // --- clouds ---
    {
 
        sprite = m_SpritesheetMarley.GetSprite(0, 46);

        static float translationCloudX, whole;
        translationCloudX = Engine::m_Engine->GetTime() / 300;
        translationCloudX = std::modf(translationCloudX, &whole) * 2.0f;
        {
            //fill index buffer object (ibo)
            m_IndexBuffer->AddObject(IndexBuffer::INDEX_BUFFER_QUAD);
            
            // model matrix
            glm::vec3 translation(translationCloudX, 0, 0);
            glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f),translation) * sprite->GetScale();
            
            //combine all matrixes
            glm::mat4 model_view_projection = m_Camera->GetViewProjectionMatrix() * modelMatrix;
            
            glm::mat4 position = model_view_projection * normalizedPosition;
            
            float pos1X = sprite->m_Pos1X; 
            float pos1Y = sprite->m_Pos1Y; 
            float pos2X = sprite->m_Pos2X;
            float pos2Y = sprite->m_Pos2Y;
            
            float textureID = static_cast<float>(m_SpritesheetMarley.GetTextureSlot());
            
            float verticies[] = 
            { /*   positions   */ /* texture coordinate */
                 position[0][0], position[0][1], pos1X, pos1Y, textureID, //    0.0f,  1.0f,
                 position[1][0], position[1][1], pos2X, pos1Y, textureID, //    1.0f,  1.0f, // position 2
                 position[2][0], position[2][1], pos2X, pos2Y, textureID, //    1.0f,  0.0f, 
                 position[3][0], position[3][1], pos1X, pos2Y, textureID  //    0.0f,  0.0f  // position 1
            };
            m_VertexBuffer->LoadBuffer(verticies, sizeof(verticies));
        }
        {
            //fill index buffer object (ibo)
            m_IndexBuffer->AddObject(IndexBuffer::INDEX_BUFFER_QUAD);

            // model matrix
            glm::vec3 translation(translationCloudX-1.0f, 0, 0);
            glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f),translation) * sprite->GetScale();

            //combine all matrixes
            glm::mat4 model_view_projection = m_Camera->GetViewProjectionMatrix() * modelMatrix;
 
            glm::mat4 position = model_view_projection * normalizedPosition;
            
            float pos1X = sprite->m_Pos1X; 
            float pos1Y = sprite->m_Pos1Y; 
            float pos2X = sprite->m_Pos2X;
            float pos2Y = sprite->m_Pos2Y;
            
            float textureID = static_cast<float>(m_SpritesheetMarley.GetTextureSlot());
    
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
    
    // --- second image ---
    {
        //fill index buffer object (ibo)
        m_IndexBuffer->AddObject(IndexBuffer::INDEX_BUFFER_QUAD);
        sprite = m_SpritesheetMarley.GetSprite(0, 47);

        // model matrix
        glm::mat4 modelMatrix = sprite->GetScale();

        //combine all matrixes
        glm::mat4 model_view_projection = m_Camera->GetViewProjectionMatrix() * modelMatrix;
 
        glm::mat4 position = model_view_projection * normalizedPosition;

        float pos1X = sprite->m_Pos1X; 
        float pos1Y = sprite->m_Pos1Y; 
        float pos2X = sprite->m_Pos2X;
        float pos2Y = sprite->m_Pos2Y;
 
        float textureID = static_cast<float>(m_SpritesheetMarley.GetTextureSlot());
 
        float verticies[] = 
        { /*   positions   */ /* texture coordinate */
             position[0][0], position[0][1], pos1X, pos1Y, textureID, //    0.0f,  1.0f,
             position[1][0], position[1][1], pos2X, pos1Y, textureID, //    1.0f,  1.0f, // position 2
             position[2][0], position[2][1], pos2X, pos2Y, textureID, //    1.0f,  0.0f, 
             position[3][0], position[3][1], pos1X, pos2Y, textureID  //    0.0f,  0.0f  // position 1
        };
        m_VertexBuffer->LoadBuffer(verticies, sizeof(verticies));
    }
 
    // --- third image ---
    if (showTabIcons)
    {
        //fill index buffer object (ibo)
        m_IndexBuffer->AddObject(IndexBuffer::INDEX_BUFFER_QUAD);
    
        sprite = m_SpritesheetMarley.GetSprite(0, 36);
        
        // model matrix
        glm::mat4 modelMatrix = sprite->GetScale();

        //combine all matrixes
        glm::mat4 model_view_projection = m_Camera->GetViewProjectionMatrix() * modelMatrix;

        glm::mat4 position = model_view_projection * normalizedPosition;
        
        float pos1X = sprite->m_Pos1X; 
        float pos1Y = sprite->m_Pos1Y; 
        float pos2X = sprite->m_Pos2X;
        float pos2Y = sprite->m_Pos2Y;
 
        float textureID = static_cast<float>(m_SpritesheetMarley.GetTextureSlot());
 
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

void MainScreenLayer::OnEvent(Event& event) 
{
}
