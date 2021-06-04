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
    m_SpritesheetHorn.AddSpritesheetAnimation("resources/sprites2/horn.png", 25 /* frames */, 500 /* milliseconds per frame */, 3.0f /* scale) */);
    m_HornAnimation = m_SpritesheetHorn.GetSpriteAnimation();
    m_HornAnimation->Start();
    
    m_SpritesheetWalk.AddSpritesheetAnimation("resources/sprites2/walk.png", 6 /* frames */, 150 /* milliseconds per frame */, 3.0f /* scale) */);
    m_WalkAnimation = m_SpritesheetWalk.GetSpriteAnimation();
    m_WalkAnimation->Start();
    
    m_Translation.z = 0.0f; // not used
    m_GuybrushWalkDelta = 33*4.3f / Engine::m_Engine->GetWindowWidth();
}

void Overlay::OnDetach() 
{
    
}

void Overlay::OnUpdate() 
{
    m_IsWalking = false;
    
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
    
    
    if (abs(leftStick.y) > 0.1)
    {
        float translationStep = m_TranslationSpeed * Engine::m_Engine->GetTimestep();
        m_Translation.y += translationStep * leftStick.y;
    }
    
    bool moveRight = false;
    if (abs(leftStick.x) > 0.1)
    {
        
        m_IsWalking = true;
        float translationStep = m_TranslationSpeed * Engine::m_Engine->GetTimestep();

        if (leftStick.x > 0)
        {
            moveRight = true;
            if (!m_WalkAnimation->IsRunning()) 
            {
                m_WalkAnimation->Start();
                m_Translation.x += m_GuybrushWalkDelta;
            }
        }
        else
        {
            moveRight = false;
            if (!m_WalkAnimation->IsRunning()) 
            {
                m_WalkAnimation->Start();
                m_Translation.x -= m_GuybrushWalkDelta;
            }
        }
        
        m_SpritesheetWalk.BeginScene();
        //fill index buffer object (ibo)
        m_IndexBuffer->AddObject(IndexBuffer::INDEX_BUFFER_QUAD);

        spriteWalk = m_WalkAnimation->GetSprite();
        
        // model matrix
        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f),m_Translation) * glm::rotate(glm::mat4(1.0f), m_Rotation, glm::vec3(0, 0, 1) ) * spriteWalk->GetScale();

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

    if (!m_IsWalking)
    {
        if (!m_HornAnimation->IsRunning()) m_HornAnimation->Start();
        m_SpritesheetHorn.BeginScene();
        //fill index buffer object (ibo)
        m_IndexBuffer->AddObject(IndexBuffer::INDEX_BUFFER_QUAD);

        sprite = m_HornAnimation->GetSprite();

        // model matrix       
        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f),m_Translation) * glm::rotate(glm::mat4(1.0f), m_Rotation, glm::vec3(0, 0, 1) ) * sprite->GetScale();
        
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
