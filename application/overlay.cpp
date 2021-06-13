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

#include <cmath> 

#include "overlay.h"
#include "input.h"
#include "renderer.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include <gtx/transform.hpp>
#include "controller.h"


float debugTranslationX = 0.0f;
float debugTranslationY = 0.0f;

void Overlay::OnAttach() 
{ 
    // horn
    m_SpritesheetHorn.AddSpritesheetAnimation(m_SpritesheetMarley->GetSprite(0, 68), 25 /* frames */, 500 /* milliseconds per frame */, 3.0f /* scale) */);
    m_HornAnimation = m_SpritesheetHorn.GetSpriteAnimation();
    m_HornAnimation->Start();
    
    m_SpritesheetWalk.AddSpritesheetAnimation(m_SpritesheetMarley->GetSprite(0, 69), 6 /* frames */, 150 /* milliseconds per frame */, 3.0f /* scale) */);
    m_WalkAnimation = m_SpritesheetWalk.GetSpriteAnimation();
    m_WalkAnimation->Start();
    m_GuybrushWalkDelta = 33*4.3f / Engine::m_Engine->GetWindowWidth();
    
    m_SpritesheetWalkUp.AddSpritesheetAnimation(m_SpritesheetMarley->GetSprite(0, 71), 6 /* frames */, 150 /* milliseconds per frame */, 3.0f /* scale) */);
    m_WalkUpAnimation = m_SpritesheetWalkUp.GetSpriteAnimation();
    m_WalkUpAnimation->Start();
    m_GuybrushWalkUpDelta = 12.0f / Engine::m_Engine->GetWindowWidth();
    
    m_SpritesheetWalkDown.AddSpritesheetAnimation(m_SpritesheetMarley->GetSprite(0, 70), 6 /* frames */, 150 /* milliseconds per frame */, 3.0f /* scale) */);
    m_WalkDownAnimation = m_SpritesheetWalkDown.GetSpriteAnimation();
    m_WalkDownAnimation->Start();
    m_GuybrushWalkDownDelta = 12.0f / Engine::m_Engine->GetWindowWidth();
    
    m_Translation.y = LIMIT_DOWN;
    m_Translation.z = 0.0f;
}

void Overlay::OnDetach() 
{
    
}

void Overlay::OnUpdate() 
{
    bool m_IsWalking = false;
    float translationStep = m_TranslationSpeed * Engine::m_Engine->GetTimestep();
    
    // rotate based on controller input
    if (Input::IsControllerButtonPressed(Controller::FIRST_CONTROLLER, Controller::BUTTON_LEFTSHOULDER)) 
    {
        m_Rotation += m_RotationSpeed * Engine::m_Engine->GetTimestep();
    }
    else if (Input::IsControllerButtonPressed(Controller::FIRST_CONTROLLER, Controller::BUTTON_RIGHTSHOULDER)) 
    {
        m_Rotation -= m_RotationSpeed * Engine::m_Engine->GetTimestep();
    }

    // translation based on controller input
    glm::vec2 leftStick  = Input::GetControllerStick(Controller::FIRST_CONTROLLER, Controller::LEFT_STICK);

    bool stickDeflectionX = (abs(leftStick.x) > 0.1) && (abs(leftStick.x) > abs(leftStick.y));
    if ( (m_FrameTranslationX != 0) && (!stickDeflectionX) )
    {
        m_Translation.x += m_FrameTranslationX;
        m_FrameTranslationX = 0.0f;
    }

    bool limitLeft = false, limitRight = false;
    bool limitUp = false, limitDown = false;

    if (m_Translation.x >= LIMIT_RIGHT) 
    {
        m_Translation.x = LIMIT_RIGHT;
        limitRight = true;
    }

    // interpolation between upper and lower x positions
    float limitLeftBeach = (m_Translation.y + 0.0558f)/(-0.1594f) * 0.3326f;
    
    if (m_Translation.x <= LIMIT_LEFT + limitLeftBeach) 
    {
        m_Translation.x = LIMIT_LEFT + limitLeftBeach;
        limitLeft = true;
    }

    // limit y and calculate depth scale
    float depth, scaleDepth;
    if (m_Translation.y > -0.0558f) 
    {
        m_Translation.y = -0.0558f;
        limitUp = true;
    }

    if (m_Translation.y <= LIMIT_DOWN) 
    {
        m_Translation.y = LIMIT_DOWN;
        limitDown = true;
    }
    depth = ((LIMIT_UP-m_Translation.y) / (-LIMIT_DOWN + LIMIT_UP)); // 0.0f to 1.0f
    scaleDepth = (1.0f + depth) * 0.5f;

    bool moveRight = false;
    bool stickDeflectionY = (abs(leftStick.y) > 0.1) && (abs(leftStick.y) > abs(leftStick.x));
    bool canMoveRight = (leftStick.x > 0) && (!limitRight);
    bool canMoveLeft = (leftStick.x < 0) && (!limitLeft);
    bool canMoveUp = (leftStick.y > 0) && (!limitUp);
    bool canMoveDown = (leftStick.y < 0) && (!limitDown);

    if ( stickDeflectionX && (canMoveLeft || canMoveRight))
    {
        m_IsWalking = true;
        const int FEEL_GOOD_FACTOR = 4;

        if (leftStick.x > 0)
        {
            moveRight = true;
            if (!m_WalkAnimation->IsRunning()) 
            {
                m_WalkAnimation->Start();
                m_Translation.x += m_GuybrushWalkDelta;
                m_FrameTranslationX = 0;
            }
            else
            {
                m_FrameTranslationX = m_GuybrushWalkDelta / static_cast<float>(m_WalkAnimation->GetFrames() + FEEL_GOOD_FACTOR) * m_WalkAnimation->GetCurrentFrame();
            }
        }
        else
        {
            moveRight = false;
            if (!m_WalkAnimation->IsRunning()) 
            {
                m_WalkAnimation->Start();
                m_Translation.x -= m_GuybrushWalkDelta;
                m_FrameTranslationX = 0;
            }
            else
            {
                m_FrameTranslationX = -m_GuybrushWalkDelta / static_cast<float>(m_WalkAnimation->GetFrames() + FEEL_GOOD_FACTOR) * m_WalkAnimation->GetCurrentFrame();
            }
        }

        m_SpritesheetWalk.BeginScene();
        //fill index buffer object (ibo)
        m_IndexBuffer->AddObject(IndexBuffer::INDEX_BUFFER_QUAD);
    
        Sprite* spriteWalk = m_WalkAnimation->GetSprite();
        
        // model matrix
        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f),m_Translation) * glm::rotate(glm::mat4(1.0f), m_Rotation, glm::vec3(0, 0, 1) ) * spriteWalk->GetScale() * scaleDepth;
    
        // --- combine model and camera matrixes into MVP matrix---
        glm::mat4 model_view_projection =  m_Camera->GetViewProjectionMatrix() * modelMatrix;
    
        glm::mat4 position  =  model_view_projection * Renderer::normalizedPosition;
    
        float pos1X; 
        float pos1Y; 
        float pos2X;
        float pos2Y;
        
        if (moveRight)
        {
            pos1X = spriteWalk->m_Pos1X; 
            pos2X = spriteWalk->m_Pos2X;
        }
        else
        {
            // flip horizontally
            pos2X = spriteWalk->m_Pos1X; 
            pos1X = spriteWalk->m_Pos2X;
        }
        pos1Y = spriteWalk->m_Pos1Y; 
        pos2Y = spriteWalk->m_Pos2Y;
    
        float textureID = static_cast<float>(m_SpritesheetWalk.GetTextureSlot());
    
        float verticies[] = 
        { /*   positions   */ /* texture coordinate */
             position[0][0], position[0][1], pos1X, pos1Y, textureID, //    0.0f,  1.0f,
             position[1][0], position[1][1], pos2X, pos1Y, textureID, //    1.0f,  1.0f, // position 2
             position[2][0], position[2][1], pos2X, pos2Y, textureID, //    1.0f,  0.0f, 
             position[3][0], position[3][1], pos1X, pos2Y, textureID  //    0.0f,  0.0f  // position 1
        };
        m_VertexBuffer->LoadBuffer(verticies, sizeof(verticies));
    }
    else
    {
        m_WalkAnimation->Start();
    }
    
    if ( stickDeflectionY && canMoveUp)
    {
        m_IsWalking = true;
        static uint prevFrame = 0;
        uint frame = m_WalkUpAnimation->GetCurrentFrame();
        if (prevFrame != frame)
        {
            m_Translation.y += m_GuybrushWalkUpDelta;
            prevFrame = frame;
        }
        if (!m_WalkUpAnimation->IsRunning()) 
        {
            m_WalkUpAnimation->Start();
        }
        m_SpritesheetWalkUp.BeginScene();
        //fill index buffer object (ibo)
        m_IndexBuffer->AddObject(IndexBuffer::INDEX_BUFFER_QUAD);
        
        Sprite* spriteWalk = m_WalkUpAnimation->GetSprite();
        
        // model matrix
        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f),m_Translation) * glm::rotate(glm::mat4(1.0f), m_Rotation, glm::vec3(0, 0, 1) ) * spriteWalk->GetScale() * scaleDepth;

        // --- combine model and camera matrixes into MVP matrix---
        glm::mat4 model_view_projection =  m_Camera->GetViewProjectionMatrix() * modelMatrix;

        glm::mat4 position  =  model_view_projection * Renderer::normalizedPosition;

        float pos1X = spriteWalk->m_Pos1X; 
        float pos1Y = spriteWalk->m_Pos1Y; 
        float pos2X = spriteWalk->m_Pos2X;
        float pos2Y = spriteWalk->m_Pos2Y;

        float textureID = static_cast<float>(m_SpritesheetWalkUp.GetTextureSlot());

        float verticies[] = 
        { /*   positions   */ /* texture coordinate */
                position[0][0], position[0][1], pos1X, pos1Y, textureID, //    0.0f,  1.0f,
                position[1][0], position[1][1], pos2X, pos1Y, textureID, //    1.0f,  1.0f, // position 2
                position[2][0], position[2][1], pos2X, pos2Y, textureID, //    1.0f,  0.0f, 
                position[3][0], position[3][1], pos1X, pos2Y, textureID  //    0.0f,  0.0f  // position 1
        };
        m_VertexBuffer->LoadBuffer(verticies, sizeof(verticies));
    }
    else
    {
        m_WalkUpAnimation->Start();
    }
    
    if ( stickDeflectionY && canMoveDown)
    {
        m_IsWalking = true;
        static uint prevFrame = 0;
        uint frame = m_WalkDownAnimation->GetCurrentFrame();
        if (prevFrame != frame)
        {
            m_Translation.y -= m_GuybrushWalkDownDelta;
            prevFrame = frame;
        }
        if (!m_WalkDownAnimation->IsRunning()) 
        {
            m_WalkDownAnimation->Start();
        }
        
        m_SpritesheetWalkDown.BeginScene();
        //fill index buffer object (ibo)
        m_IndexBuffer->AddObject(IndexBuffer::INDEX_BUFFER_QUAD);
        
        Sprite* spriteWalk = m_WalkDownAnimation->GetSprite();
        
        // model matrix
        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f),m_Translation) * glm::rotate(glm::mat4(1.0f), m_Rotation, glm::vec3(0, 0, 1) ) * spriteWalk->GetScale() * scaleDepth;

        // --- combine model and camera matrixes into MVP matrix---
        glm::mat4 model_view_projection =  m_Camera->GetViewProjectionMatrix() * modelMatrix;

        glm::mat4 position  =  model_view_projection * Renderer::normalizedPosition;

        float pos1X = spriteWalk->m_Pos1X; 
        float pos1Y = spriteWalk->m_Pos1Y; 
        float pos2X = spriteWalk->m_Pos2X;
        float pos2Y = spriteWalk->m_Pos2Y;

        float textureID = static_cast<float>(m_SpritesheetWalkDown.GetTextureSlot());

        float verticies[] = 
        { /*   positions   */ /* texture coordinate */
                position[0][0], position[0][1], pos1X, pos1Y, textureID, //    0.0f,  1.0f,
                position[1][0], position[1][1], pos2X, pos1Y, textureID, //    1.0f,  1.0f, // position 2
                position[2][0], position[2][1], pos2X, pos2Y, textureID, //    1.0f,  0.0f, 
                position[3][0], position[3][1], pos1X, pos2Y, textureID  //    0.0f,  0.0f  // position 1
        };
        m_VertexBuffer->LoadBuffer(verticies, sizeof(verticies));
    }
    else
    {
        m_WalkDownAnimation->Start();
    }

    if (!m_IsWalking)
    {
        if (!m_HornAnimation->IsRunning()) m_HornAnimation->Start();
        m_SpritesheetHorn.BeginScene();
        //fill index buffer object (ibo)
        m_IndexBuffer->AddObject(IndexBuffer::INDEX_BUFFER_QUAD);
    
        Sprite* sprite = m_HornAnimation->GetSprite();
    
        // model matrix       
        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f),m_Translation) * glm::rotate(glm::mat4(1.0f), m_Rotation, glm::vec3(0, 0, 1) ) * sprite->GetScale() * scaleDepth;
        
        // --- combine model and camera matrixes into MVP matrix---
        glm::mat4 model_view_projection =  m_Camera->GetViewProjectionMatrix() * modelMatrix;
        
        glm::mat4 position  =  model_view_projection * Renderer::normalizedPosition;
        
        float pos1X = sprite->m_Pos1X; 
        float pos1Y = sprite->m_Pos1Y; 
        float pos2X = sprite->m_Pos2X;
        float pos2Y = sprite->m_Pos2Y;
        
        float textureID = static_cast<float>(m_SpritesheetHorn.GetTextureSlot());
        
        float verticies[] = 
        { /*   positions   */ /* texture coordinate */
             position[0][0], position[0][1], pos1X, pos1Y, textureID, //    0.0f,  1.0f,
             position[1][0], position[1][1], pos2X, pos1Y, textureID, //    1.0f,  1.0f, // position 2
             position[2][0], position[2][1], pos2X, pos2Y, textureID, //    1.0f,  0.0f, 
             position[3][0], position[3][1], pos1X, pos2Y, textureID  //    0.0f,  0.0f  // position 1
        };
        m_VertexBuffer->LoadBuffer(verticies, sizeof(verticies));
    }
    else
    {
        m_HornAnimation->Start();
    }
}

void Overlay::OnEvent(Event& event) 
{
    
    EventDispatcher dispatcher(event);
        
    dispatcher.Dispatch<ControllerButtonPressedEvent>([this](ControllerButtonPressedEvent event) 
        {
            OnControllerButtonPressed(event);
            return true;
        }
    );
        
    dispatcher.Dispatch<ControllerButtonReleasedEvent>([this](ControllerButtonReleasedEvent event) 
        {
            OnControllerButtonReleased(event);
            return true;
        }
    );

}

void Overlay::OnControllerButtonPressed(ControllerButtonPressedEvent& event)
{
    event.MarkAsHandled();
}

void Overlay::OnControllerButtonReleased(ControllerButtonReleasedEvent& event)
{
    event.MarkAsHandled();
}
