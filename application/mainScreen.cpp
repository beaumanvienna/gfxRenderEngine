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
#include "renderer.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

bool showTabIcons = true;
float duration = 1.0f, previousDuration = 1.0f;

void MainScreenLayer::OnAttach() 
{
    m_TranslationSpeedClouds = 0.001f;
    InitTabAnimation();
}

void MainScreenLayer::InitTabAnimation()
{
    // move left to center
    tabAnimation.AddTranslation(Translation(1.0f * duration, -1400.0f, 0.0f));
    tabAnimation.AddRotation(Rotation(1.0f * duration, 0.0f, 0.0f));        // idle
    tabAnimation.AddScale(Scale(0.9f * duration, 0.6f, 0.6f));
    tabAnimation.AddScale(Scale(0.1f * duration, 0.6f, 1.0f));
    
    //wiggle
    const float rotationTiming = 0.75f;
    tabAnimation.AddTranslation(Translation(1.0f * duration * rotationTiming, 0.0f, 0.0f));  // idle
    tabAnimation.AddRotation(Rotation(0.1f * duration * rotationTiming,  0.0f,  0.2f));
    tabAnimation.AddRotation(Rotation(0.2f * duration * rotationTiming,  0.2f, -0.2f));
    tabAnimation.AddRotation(Rotation(0.2f * duration * rotationTiming, -0.2f,  0.2f));
    tabAnimation.AddRotation(Rotation(0.2f * duration * rotationTiming,  0.2f, -0.1f));
    tabAnimation.AddRotation(Rotation(0.2f * duration * rotationTiming, -0.1f,  0.1f));
    tabAnimation.AddRotation(Rotation(0.1f * duration * rotationTiming,  0.1f,  0.0f));
    tabAnimation.AddScale(Scale(1.0f * duration * rotationTiming,        1.0f,  1.0f));      // idle
    
    // idle
    tabAnimation.AddTranslation(Translation(0.5f * duration, 0.0f, 0.0f));  // idle
    tabAnimation.AddRotation(Rotation(0.4f * duration,  0.0f, 0.0f));       // idle
    tabAnimation.AddScale(Scale(0.5f * duration, 1.0f, 1.0f));              //idle
    
    // move center to left
    tabAnimation.AddTranslation(Translation(1.0f * duration, 0.0f, -1400.0f));
    tabAnimation.AddRotation(Rotation(0.1f * duration, -0.05f, 0.0f));
    tabAnimation.AddRotation(Rotation(0.9f * duration,  0.0f, 0.0f));       // idle
    tabAnimation.AddScale(Scale(0.1f * duration, 1.0f, 0.6f));
    tabAnimation.AddScale(Scale(0.9f * duration, 0.6f, 0.6f));
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
    m_SpritesheetMarley->BeginScene();
    
    // --- clouds ---
    {
 
        Sprite* sprite = m_SpritesheetMarley->GetSprite(0, 46);
        static float translationNormalizedX, whole, translationCloudX;
        translationNormalizedX = Engine::m_Engine->GetTime() / 300;
        translationNormalizedX = std::modf(translationNormalizedX, &whole);

        translationCloudX = translationNormalizedX * 1920.0f;

        {
            float pos1X = sprite->m_Pos1X; 
            float pos1Y = sprite->m_Pos1Y; 
            float pos2X = sprite->m_Pos2X;
            float pos2Y = sprite->m_Pos2Y;
            
            glm::mat4 spriteMatrix  = glm::mat4
            (
                0.0f,             sprite->m_Height, 1.0f, 1.0f,
                sprite->m_Width,  sprite->m_Height, 1.0f, 1.0f,
                sprite->m_Width,  0.0f,             1.0f, 1.0f,
                0.0f,             0.0f,             1.0f, 1.0f
            );
            
            // model matrix
            glm::vec3 scale(1.5f,1.5f,0.0f);
            glm::vec3 translation(translationCloudX-960.0f, -540.0f, 0);
            glm::mat4 position = glm::translate(glm::mat4(1.0f),translation) * glm::scale(glm::mat4(1.0f), scale) * spriteMatrix;
            
            //fill index buffer object (ibo)
            m_IndexBuffer->AddObject(IndexBuffer::INDEX_BUFFER_QUAD);
            
            float textureID = static_cast<float>(m_SpritesheetMarley->GetTextureSlot());
            
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
            float pos1X = sprite->m_Pos1X; 
            float pos1Y = sprite->m_Pos1Y; 
            float pos2X = sprite->m_Pos2X;
            float pos2Y = sprite->m_Pos2Y;
            
            glm::mat4 spriteMatrix  = glm::mat4
            (
                0.0f,             sprite->m_Height, 1.0f, 1.0f,
                sprite->m_Width,  sprite->m_Height, 1.0f, 1.0f,
                sprite->m_Width,  0.0f,             1.0f, 1.0f,
                0.0f,             0.0f,             1.0f, 1.0f
            );
        
            // model matrix
            glm::vec3 scale(1.5f,1.5f,0.0f);
            glm::vec3 translation(translationCloudX-960.0f-1920.0f, -540.0f, 0);
            glm::mat4 position = glm::translate(glm::mat4(1.0f),translation) * glm::scale(glm::mat4(1.0f), scale) * spriteMatrix;

            //fill index buffer object (ibo)
            m_IndexBuffer->AddObject(IndexBuffer::INDEX_BUFFER_QUAD);
            
            float textureID = static_cast<float>(m_SpritesheetMarley->GetTextureSlot());
        
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
        Sprite* sprite = m_SpritesheetMarley->GetSprite(0, 47);
        
        float pos1X = sprite->m_Pos1X; 
        float pos1Y = sprite->m_Pos1Y; 
        float pos2X = sprite->m_Pos2X;
        float pos2Y = sprite->m_Pos2Y;
        
        glm::mat4 spriteMatrix  = glm::mat4
        (
            0.0f,             sprite->m_Height, 1.0f, 1.0f,
            sprite->m_Width,  sprite->m_Height, 1.0f, 1.0f,
            sprite->m_Width,  0.0f,             1.0f, 1.0f,
            0.0f,             0.0f,             1.0f, 1.0f
        );

        // model matrix
        glm::vec3 scale(1.5f,1.5f,0.0f);
        glm::vec3 translation(-960.0f,-540.0f,0.0f);
        glm::mat4 position = glm::translate(glm::mat4(1.0f),translation) * glm::scale(glm::mat4(1.0f), scale) * spriteMatrix;

        //fill index buffer object (ibo)
        m_IndexBuffer->AddObject(IndexBuffer::INDEX_BUFFER_QUAD);
 
        float textureID = static_cast<float>(m_SpritesheetMarley->GetTextureSlot());
 
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
    
        Sprite* sprite = m_SpritesheetMarley->GetSprite(0, 36);
        
        // model matrix
        if (duration != previousDuration)
        {
            tabAnimation.Reset();
            InitTabAnimation();
        }
        previousDuration = duration;
        if (!tabAnimation.IsRunning()) tabAnimation.Start();
        auto animationMatrix = tabAnimation.GetTransformation();
        
        float pos1X = sprite->m_Pos1X; 
        float pos1Y = sprite->m_Pos1Y; 
        float pos2X = sprite->m_Pos2X;
        float pos2Y = sprite->m_Pos2Y;
        
        glm::mat4 spriteMatrix  = glm::mat4
        (
            0.0f,             sprite->m_Height, 1.0f, 1.0f,
            sprite->m_Width,  sprite->m_Height, 1.0f, 1.0f,
            sprite->m_Width,  0.0f,             1.0f, 1.0f,
            0.0f,             0.0f,             1.0f, 1.0f
        );

        // model matrix
        glm::vec3 scale(1.0f,1.0f,0.0f);
        glm::vec3 translation(-(sprite->m_Width/2.0f),0.0f,0.0f);
        glm::mat4 position = animationMatrix * glm::translate(glm::mat4(1.0f),translation) * glm::scale(glm::mat4(1.0f), scale) * spriteMatrix;

        //fill index buffer object (ibo)
        m_IndexBuffer->AddObject(IndexBuffer::INDEX_BUFFER_QUAD);
 
        float textureID = static_cast<float>(m_SpritesheetMarley->GetTextureSlot());
 
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
