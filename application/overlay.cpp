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
    m_SpritesheetHorn.AddSpritesheetAnimation("resources/sprites2/horn.png", 25, 500);
    m_HornAnimation = m_SpritesheetHorn.GetSpriteAnimation();
    m_HornAnimation->Start();
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
    glm::vec3 translation(0, 0, 0);
    constexpr float amplifiction = 0.5f;

    // translation based on controller input
    glm::vec2 leftStick  = Input::GetControllerStick(Controller::FIRST_CONTROLLER, Controller::LEFT_STICK);
    glm::vec2 rightStick = Input::GetControllerStick(Controller::FIRST_CONTROLLER, Controller::RIGHT_STICK);
    
    debugTranslationX += amplifiction * leftStick.x;
    debugTranslationY += amplifiction * leftStick.y;
    
    debugTranslationX += amplifiction * rightStick.x;
    debugTranslationY += amplifiction * rightStick.y;
    
    translation.x =  4.5f  + debugTranslationX;
    translation.y = -6.0f + debugTranslationY;
    glm::mat4  modelMatrix = sprite->GetScale() * glm::translate(glm::mat4(1.0f),translation);

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
    glm::vec4 position1 = model_view_projection * normalizedPosition[0];
    glm::vec4 position2 = model_view_projection * normalizedPosition[1];
    glm::vec4 position3 = model_view_projection * normalizedPosition[2];
    glm::vec4 position4 = model_view_projection * normalizedPosition[3];

    float pos1X = sprite->m_Pos1X; 
    float pos1Y = sprite->m_Pos1Y; 
    float pos2X = sprite->m_Pos2X;
    float pos2Y = sprite->m_Pos2Y;

    float textureID = static_cast<float>(m_SpritesheetHorn.GetTextureSlot());

    float verticies[] = 
    { /*   positions   */ /* texture coordinate */
         position1[0], position1[1], pos1X, pos1Y, textureID, //    0.0f,  1.0f,
         position2[0], position2[1], pos2X, pos1Y, textureID, //    1.0f,  1.0f, // position 2
         position3[0], position3[1], pos2X, pos2Y, textureID, //    1.0f,  0.0f, 
         position4[0], position4[1], pos1X, pos2Y, textureID  //    0.0f,  0.0f  // position 1
    };
    m_VertexBuffer->LoadBuffer(verticies, sizeof(verticies));
}

void Overlay::OnEvent(Event& event) 
{
    
    //if (event.GetCategoryFlags() & EventCategoryApplication) LOG_APP_INFO(event);
    //if (event.GetCategoryFlags() & EventCategoryInput)       LOG_APP_INFO(event);
    //if (event.GetCategoryFlags() & EventCategoryMouse)       LOG_APP_INFO(event);
    //if (event.GetCategoryFlags() & EventCategoryController)  LOG_APP_INFO(event);
    //if (event.GetCategoryFlags() & EventCategoryJoystick)    LOG_APP_INFO(event);    
    
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
    float rotate = 0.0f;
    
    
    switch(event.GetControllerButton())
    {
        case Controller::BUTTON_LEFTSHOULDER:
            rotate = -0.1f;
            break;
        case Controller::BUTTON_RIGHTSHOULDER:
            rotate = +0.1f;
            break;
    }
    
    if (rotate)
    {
        float rotation = m_Camera->GetRotation();
        rotation += rotate;
        m_Camera->SetRotation(rotation);
    }
    
    event.MarkAsHandled();
}

void Overlay::OnControllerButtonReleased(ControllerButtonReleasedEvent& event)
{
    event.MarkAsHandled();
}
