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
#include "GL.h"

bool showTabIcons = true;

void MainScreenLayer::OnAttach() 
{ 
    INIT_LAYER();
    
    m_SpritesheetMarley.AddSpritesheetPPSSPP("resources/images/ui_atlas/ui_atlas.png");
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
    m_SpritesheetMarley.BeginDrawCall();
    
    // --- clouds ---
    {
 
        sprite = m_SpritesheetMarley.GetSprite(0, 46);
 
        pos1X = sprite->m_Pos1X; 
        pos1Y = sprite->m_Pos1Y; 
        pos2X = sprite->m_Pos2X;
        pos2Y = sprite->m_Pos2Y;
 
        // aspect ratio of image
        scaleTextureY = sprite->m_Width / (1.0f * sprite->m_Height);
 
        // scale to original size
        scaleSize = Engine::m_Engine->GetWindowWidth() / (1.0f * sprite->m_Width);
 
        // scale to original size
        orthoLeft   = ortho_left   * scaleTextureX * scaleSize;
        orthoRight  = ortho_right  * scaleTextureX * scaleSize;
        orthoBottom = ortho_bottom * scaleTextureY * scaleSize;
        orthoTop    = ortho_top    * scaleTextureY * scaleSize;
 
        glm::mat4 projectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, ortho_near, ortho_far);
        
        static float cloudX, whole;
        
        cloudX = glfwGetTime() / 300;
        cloudX = std::modf(cloudX, &whole) * 2.0f;
        
        {
            //fill index buffer object (ibo)
            m_IndexBuffer->AddObject(IndexBuffer::INDEX_BUFFER_QUAD);
            
            // model matrix
            glm::mat4 modelMatrix(1.0f);
            
            //view matrix
            glm::mat4 viewMatrix(1.0f);
            
            glm::vec3 translation(cloudX, 0, 0);
 
            modelMatrix = glm::translate(glm::mat4(1.0f),translation);
 
            //combine all matrixes
            glm::mat4 model_view_projection = modelMatrix * viewMatrix * projectionMatrix;
 
            position1 = model_view_projection * normalizedPosition[0];
            position2 = model_view_projection * normalizedPosition[1];
            position3 = model_view_projection * normalizedPosition[2];
            position4 = model_view_projection * normalizedPosition[3];
            
            float textureID = static_cast<float>(m_SpritesheetMarley.GetTextureSlot());
    
            float verticies[] = 
            { /*   positions   */ /* texture coordinate */
                 position1[0], position1[1], pos1X, pos1Y, textureID, //    0.0f,  1.0f,
                 position2[0], position2[1], pos2X, pos1Y, textureID, //    1.0f,  1.0f, // position 2
                 position3[0], position3[1], pos2X, pos2Y, textureID, //    1.0f,  0.0f, 
                 position4[0], position4[1], pos1X, pos2Y, textureID  //    0.0f,  0.0f  // position 1
            };
            m_VertexBuffer->LoadBuffer(verticies, sizeof(verticies));
        }
        {
            //fill index buffer object (ibo)
            m_IndexBuffer->AddObject(IndexBuffer::INDEX_BUFFER_QUAD);
            
            // model matrix
            glm::mat4 modelMatrix(1.0f);
            
            //view matrix
            glm::mat4 viewMatrix(1.0f);
            
            glm::vec3 translation(cloudX-2.0f, 0, 0);
 
            modelMatrix = glm::translate(glm::mat4(1.0f),translation);
 
            //combine all matrixes
            glm::mat4 model_view_projection = modelMatrix * viewMatrix * projectionMatrix;
 
            position1 = model_view_projection * normalizedPosition[0];
            position2 = model_view_projection * normalizedPosition[1];
            position3 = model_view_projection * normalizedPosition[2];
            position4 = model_view_projection * normalizedPosition[3];
            
            float textureID = static_cast<float>(m_SpritesheetMarley.GetTextureSlot());
    
            float verticies[] = 
            { /*   positions   */ /* texture coordinate */
                 position1[0], position1[1], pos1X, pos1Y, textureID, //    0.0f,  1.0f,
                 position2[0], position2[1], pos2X, pos1Y, textureID, //    1.0f,  1.0f, // position 2
                 position3[0], position3[1], pos2X, pos2Y, textureID, //    1.0f,  0.0f, 
                 position4[0], position4[1], pos1X, pos2Y, textureID  //    0.0f,  0.0f  // position 1
            };
            m_VertexBuffer->LoadBuffer(verticies, sizeof(verticies));
        }
    }
    
    // --- second image ---
    {
        //fill index buffer object (ibo)
        m_IndexBuffer->AddObject(IndexBuffer::INDEX_BUFFER_QUAD);
        sprite = m_SpritesheetMarley.GetSprite(0, 47);
 
        pos1X = sprite->m_Pos1X; 
        pos1Y = sprite->m_Pos1Y; 
        pos2X = sprite->m_Pos2X;
        pos2Y = sprite->m_Pos2Y;
 
        // aspect ratio of image
        scaleTextureY = sprite->m_Width / (1.0f * sprite->m_Height);
 
        // scale to original size
        scaleSize = Engine::m_Engine->GetWindowWidth() / (1.0f * sprite->m_Width);
 
        // scale to original size
        orthoLeft   = ortho_left   * scaleTextureX * scaleSize;
        orthoRight  = ortho_right  * scaleTextureX * scaleSize;
        orthoBottom = ortho_bottom * scaleTextureY * scaleSize;
        orthoTop    = ortho_top    * scaleTextureY * scaleSize;
 
        glm::mat4 projectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, ortho_near, ortho_far);
 
        //combine all matrixes
        glm::mat4 model_view_projection = projectionMatrix;
 
        position1 = model_view_projection * normalizedPosition[0];
        position2 = model_view_projection * normalizedPosition[1];
        position3 = model_view_projection * normalizedPosition[2];
        position4 = model_view_projection * normalizedPosition[3];
 
        float textureID = static_cast<float>(m_SpritesheetMarley.GetTextureSlot());
 
        float verticies[] = 
        { /*   positions   */ /* texture coordinate */
             position1[0], position1[1], pos1X, pos1Y, textureID, //    0.0f,  1.0f,
             position2[0], position2[1], pos2X, pos1Y, textureID, //    1.0f,  1.0f, // position 2
             position3[0], position3[1], pos2X, pos2Y, textureID, //    1.0f,  0.0f, 
             position4[0], position4[1], pos1X, pos2Y, textureID  //    0.0f,  0.0f  // position 1
        };
        m_VertexBuffer->LoadBuffer(verticies, sizeof(verticies));
    }
 
    // --- third image ---
    if (showTabIcons)
    {
        //fill index buffer object (ibo)
        m_IndexBuffer->AddObject(IndexBuffer::INDEX_BUFFER_QUAD);
    
        sprite = m_SpritesheetMarley.GetSprite(0, 36);
    
        pos1X = sprite->m_Pos1X; 
        pos1Y = sprite->m_Pos1Y; 
        pos2X = sprite->m_Pos2X;
        pos2Y = sprite->m_Pos2Y;
    
        // aspect ratio of image
        scaleTextureY = sprite->m_Width / (1.0f * sprite->m_Height);
    
        // scale to original size
        scaleSize = Engine::m_Engine->GetWindowWidth() / (1.0f * sprite->m_Width);
    
        // scale to original size
        orthoLeft   = ortho_left   * scaleTextureX * scaleSize;
        orthoRight  = ortho_right  * scaleTextureX * scaleSize;
        orthoBottom = ortho_bottom * scaleTextureY * scaleSize;
        orthoTop    = ortho_top    * scaleTextureY * scaleSize;
        
        
        // model matrix
        glm::mat4 modelMatrix(1.0f);
        
        //view matrix
        glm::mat4 viewMatrix(1.0f);
    
        glm::mat4 projectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, ortho_near, ortho_far);
    
        glm::vec3 translation(0, 0, 0);
        
        modelMatrix = glm::translate(glm::mat4(1.0f),translation);
 
        //combine all matrixes
        glm::mat4 model_view_projection = modelMatrix * viewMatrix * projectionMatrix;
    
        position1 = model_view_projection * normalizedPosition[0];
        position2 = model_view_projection * normalizedPosition[1];
        position3 = model_view_projection * normalizedPosition[2];
        position4 = model_view_projection * normalizedPosition[3];
    
        float textureID = static_cast<float>(m_SpritesheetMarley.GetTextureSlot());
    
        float verticies[] = 
        { /*   positions   */ /* texture coordinate */
             position1[0], position1[1], pos1X, pos1Y, textureID, //    0.0f,  1.0f,
             position2[0], position2[1], pos2X, pos1Y, textureID, //    1.0f,  1.0f, // position 2
             position3[0], position3[1], pos2X, pos2Y, textureID, //    1.0f,  0.0f, 
             position4[0], position4[1], pos1X, pos2Y, textureID  //    0.0f,  0.0f  // position 1
        };
        m_VertexBuffer->LoadBuffer(verticies, sizeof(verticies));
    }
}

void MainScreenLayer::OnEvent(Event& event) 
{
}
