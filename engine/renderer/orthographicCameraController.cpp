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

#include "orthographicCameraController.h"
#include "core.h"
#include "input.h"

float zoomFactor = 1.0f, zoomFactorPrevious = 1.0f;

OrthographicCameraController::OrthographicCameraController(std::shared_ptr<OrthographicCamera>& camera)
    : m_Camera(camera), m_CameraEnable(false)
{
    // camera
    SetProjection();
    
    m_Translation = glm::vec2(0.0f, 0.0f);
    m_Rotation = 0.0f;
}

void OrthographicCameraController::SetZoomFactor(float factor)
{
    zoomFactor = factor;
    SetProjection();
}

void OrthographicCameraController::SetRotation(float rotation)
{
    m_Rotation = rotation;
    m_Camera->SetRotation( m_Rotation );
}

void OrthographicCameraController::SetTranslation(glm::vec2 translation)
{
    m_Translation = translation;
    m_Camera->SetPosition( {m_Translation.x, m_Translation.y, 0.0f} );
}

void OrthographicCameraController::SetProjection()
{
    /* orthographic matrix for projecting two-dimensional coordinates onto the screen */

    /* normalize to -1.0f - 1.0f */
    float normalize = Engine::m_Engine->GetContextWidth()/2.0f;

    /* aspect ratio of main window */
    float aspectRatio = Engine::m_Engine->GetWindowAspectRatio();

    
    float ortho_left   =  -normalize;
    float ortho_right  =   normalize;
    float ortho_bottom =  -normalize / aspectRatio;
    float ortho_top    =   normalize / aspectRatio;
    float ortho_near   =  1.0f;
    float ortho_far    = -1.0f;

    m_Camera->SetProjection(ortho_left * zoomFactor, ortho_right * zoomFactor, ortho_bottom * zoomFactor, ortho_top * zoomFactor, ortho_near, ortho_far);
}

void OrthographicCameraController::OnUpdate()
{
    // camera control
    if (!m_CameraEnable) return;
    // move left and right based on controller input
    float cameraXMovement = 0.0f, cameraYMovement = 0.0f;
    
    if (Input::IsKeyPressed(KeyCode::ENGINE_KEY_J))
    {
        cameraXMovement = +m_TranslationSpeed * Engine::m_Engine->GetTimestep();
    }
    else if (Input::IsKeyPressed(KeyCode::ENGINE_KEY_L))
    {
        cameraXMovement = -m_TranslationSpeed * Engine::m_Engine->GetTimestep();
    }
    else if (Input::IsKeyPressed(KeyCode::ENGINE_KEY_I))
    {
        cameraYMovement = -m_TranslationSpeed * Engine::m_Engine->GetTimestep();
    }
    else if (Input::IsKeyPressed(KeyCode::ENGINE_KEY_K))
    {
        cameraYMovement = +m_TranslationSpeed * Engine::m_Engine->GetTimestep();
    }

    if (cameraXMovement || cameraYMovement)
    {
        m_Translation.x += cameraXMovement;
        m_Translation.y += cameraYMovement;
        m_Camera->SetPosition( {m_Translation.x, m_Translation.y, 0.0f} );
    }
    
    // rotate based on controller input
    float cameraRotation = 0.0f;
    float leftTrigger  = Input::GetControllerTrigger(Controller::FIRST_CONTROLLER, Controller::LEFT_TRIGGER);
    float rightTrigger = Input::GetControllerTrigger(Controller::FIRST_CONTROLLER, Controller::RIGHT_TRIGGER);
    
    if (leftTrigger)
    {
        cameraRotation = -m_RotationSpeed * Engine::m_Engine->GetTimestep();
    }
    else if (rightTrigger) 
    {
        cameraRotation = +m_RotationSpeed * Engine::m_Engine->GetTimestep();
    }
    if (cameraRotation)
    {
        m_Rotation += cameraRotation;
        m_Camera->SetRotation( m_Rotation );
    }
    // zoom factor
    if (zoomFactor != zoomFactorPrevious)
    {
        SetProjection();
    }
    zoomFactorPrevious = zoomFactor;
}

