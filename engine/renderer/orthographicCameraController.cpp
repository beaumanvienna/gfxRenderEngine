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

OrthographicCameraController::OrthographicCameraController(std::shared_ptr<OrthographicCamera>& camera)
    : m_Camera(camera)
{
    // camera
    /* orthographic matrix for projecting two-dimensional coordinates onto the screen */

    /* normalize to -0.5f - 0.5f */
    float normalizeX = 0.5f;
    float normalizeY = 0.5f;

    /* aspect ratio of main window */
    float aspectRatio = Engine::m_Engine->GetWindowAspectRatio();

    /* scale it to always have the same physical size on the screen */
    /* independently of the resolution */
    float scaleResolution = 1.0f / Engine::m_Engine->GetWindowScale();

    float ortho_left   =-normalizeX * scaleResolution;
    float ortho_right  = normalizeX * scaleResolution;
    float ortho_bottom =-normalizeY * scaleResolution / aspectRatio;
    float ortho_top    = normalizeY * scaleResolution / aspectRatio;
    float ortho_near   =  1.0f;
    float ortho_far    = -1.0f;

    m_Camera->SetProjection(ortho_left, ortho_right, ortho_bottom, ortho_top, ortho_near, ortho_far);

    m_TranslationX = 0.0f;
    m_Rotation = 0.0f;
}

void OrthographicCameraController::OnUpdate()
{
    // camera control
    // move left and right based on controller input
    float cameraXMovement = 0.0f;
    if (Input::IsControllerButtonPressed(Controller::FIRST_CONTROLLER, Controller::BUTTON_DPAD_LEFT)) 
    {
        cameraXMovement = +m_TranslationSpeed * Engine::m_Engine->GetTimestep();
    }
    else if (Input::IsControllerButtonPressed(Controller::FIRST_CONTROLLER, Controller::BUTTON_DPAD_RIGHT)) 
    {
        cameraXMovement = -m_TranslationSpeed * Engine::m_Engine->GetTimestep();
    }
    if (cameraXMovement)
    {
        m_TranslationX += cameraXMovement;
        m_Camera->SetPosition( {m_TranslationX, 0.0f, 0.0f} );
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
}

