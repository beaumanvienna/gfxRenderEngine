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

#include <chrono>

#include "core.h"
#include "input.h"
#include "controller.h"
#include "marley/marley.h"
#include "marley/appInput.h"
#include "marley/characters/InputHandler.h"

namespace MarleyApp
{
    InputHandler::InputHandler(float rotationSpeed)
        : m_RotationSpeed(rotationSpeed)
    {}

    void InputHandler::GetMovement(glm::vec2& movementCommand)
    {
        glm::vec2 movementInput = glm::vec2(0.0f, 0.0f);
        if (Marley::m_GameState->UserInputIsInabled())
        {
            // translation based on keyboard/controller input
            movementInput = AppInput::GetMovementInput();
    
            if ((abs(movementInput.x) > 0.1) && (abs(movementInput.x) > abs(movementInput.y)))
            {
                movementCommand = glm::vec2(movementInput.x, 0.0f);
            }
            else if ((abs(movementInput.y) > 0.1) && (abs(movementInput.y) > abs(movementInput.x)))
            {
                movementCommand = glm::vec2(0.0f, movementInput.y);
            }
            else
            {
                movementCommand = glm::vec2(0.0f, 0.0f);
            }
        }

        // destination-controlled command
        Marley::m_AutoMoveCharacter->GetMovement(AutoMove::MOVE_TO_DESTINATION, movementCommand);

        // generate idle event / reset move to destination
        constexpr auto IDLE_TIME = 4s;

        auto now = std::chrono::steady_clock::now();
        if (movementCommand.x + movementCommand.y)
        {
            m_IdleTimeStart = now;
        }
        Marley::m_GameState->InputIdle((now - m_IdleTimeStart) > IDLE_TIME);

        // application-controlled command
        Marley::m_AutoMoveCharacter->GetMovement(AutoMove::MOVE_APP_CONTROLLED, movementCommand);
    }

    void InputHandler::GetRotation(float& rotation)
    {
        // rotate based on controller input
        if (Input::IsControllerButtonPressed(Controller::FIRST_CONTROLLER, Controller::BUTTON_LEFTSHOULDER))
        {
            rotation += m_RotationSpeed * Engine::m_Engine->GetTimestep();
        }
        else if (Input::IsControllerButtonPressed(Controller::FIRST_CONTROLLER, Controller::BUTTON_RIGHTSHOULDER))
        {
            rotation -= m_RotationSpeed * Engine::m_Engine->GetTimestep();
        }
    }
}
