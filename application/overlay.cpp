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
    
    normalizedPosition  = glm::mat4
    (
        -0.5f,  0.5f, 1.0f, 1.0f,
         0.5f,  0.5f, 1.0f, 1.0f,
         0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, 1.0f, 1.0f
    );
}

void Overlay::OnDetach() 
{
    
}

void Overlay::OnUpdate() 
{
    
    if (!m_HornAnimation->IsRunning()) m_HornAnimation->Start();
    m_SpritesheetHorn.BeginScene();
    //fill index buffer object (ibo)
    m_IndexBuffer->AddObject(IndexBuffer::INDEX_BUFFER_QUAD);

    sprite = m_HornAnimation->GetSprite();
    
    // --- model matrix ---    
    float translationStep = m_TranslationSpeed * Engine::m_Engine->GetTimestep();

    // translation based on controller input
    glm::vec2 leftStick  = Input::GetControllerStick(Controller::FIRST_CONTROLLER, Controller::LEFT_STICK);
    glm::vec2 rightStick = Input::GetControllerStick(Controller::FIRST_CONTROLLER, Controller::RIGHT_STICK);
    
    debugTranslationX += translationStep * leftStick.x;
    debugTranslationY += translationStep * leftStick.y;
    
    debugTranslationX += translationStep * rightStick.x;
    debugTranslationY += translationStep * rightStick.y;
    
    m_Translation.x =  4.5f  + debugTranslationX;
    m_Translation.y = -6.0f + debugTranslationY;
    
    // rotate based on controller input
    if (Input::IsControllerButtonPressed(Controller::FIRST_CONTROLLER, Controller::BUTTON_LEFTSHOULDER)) 
    {
        m_Rotation -= m_RotationSpeed * Engine::m_Engine->GetTimestep();
    }
    else if (Input::IsControllerButtonPressed(Controller::FIRST_CONTROLLER, Controller::BUTTON_RIGHTSHOULDER)) 
    {
        m_Rotation += m_RotationSpeed * Engine::m_Engine->GetTimestep();
    }
    
    glm::mat4 modelMatrix = sprite->GetScale() * glm::translate(glm::mat4(1.0f),m_Translation) * glm::rotate(glm::mat4(1.0f), m_Rotation, glm::vec3(0, 0, 1) );

    // --- combine model and camera matrixes into MVP matrix---
    glm::mat4 model_view_projection = modelMatrix * m_Camera->GetViewProjectionMatrix();
    
    glm::mat4 position  = model_view_projection * normalizedPosition;

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
