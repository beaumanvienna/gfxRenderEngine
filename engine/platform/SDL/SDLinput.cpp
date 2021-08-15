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

#include "input.h"
#include "controllerEvent.h"

Controller* Input::m_Controller;

void Input::Start(Controller* controller)
{
    m_Controller = controller;
}

void Input::StartControllerConfig(int controllerID)
{
    m_Controller->StartConfig(controllerID);
}

glm::vec2 Input::GetControllerStick(const int indexID, Controller::ControllerSticks stick)
{
    float x = 0;
    float y = 0;
    if (m_Controller->GetCount() && !m_Controller->ConfigIsRunning())
    {
        auto gameController = m_Controller->GetGameController(indexID);
    
        if (stick == Controller::LEFT_STICK)
        {
            x = SDL_GameControllerGetAxis(gameController, static_cast<SDL_GameControllerAxis>(Controller::LEFT_STICK_HORIZONTAL)) / (1.0f * 32768);
            y = -SDL_GameControllerGetAxis(gameController, static_cast<SDL_GameControllerAxis>(Controller::LEFT_STICK_VERTICAL))   / (1.0f * 32768);
        }
        else if (stick == Controller::RIGHT_STICK)
        {
            x = SDL_GameControllerGetAxis(gameController, static_cast<SDL_GameControllerAxis>(Controller::RIGHT_STICK_HORIZONTAL)) / (1.0f * 32768);
            y = -SDL_GameControllerGetAxis(gameController, static_cast<SDL_GameControllerAxis>(Controller::RIGHT_STICK_VERTICAL))   / (1.0f * 32768);
        }
    }

    return { x, y };
}

float Input::GetControllerTrigger(const int indexID, Controller::Axis trigger)
{
    float x = 0;
    if (m_Controller->GetCount() && !m_Controller->ConfigIsRunning())
    {
        auto gameController = m_Controller->GetGameController(indexID);
    
        if (trigger == Controller::LEFT_TRIGGER)
        {
            x = SDL_GameControllerGetAxis(gameController, static_cast<SDL_GameControllerAxis>(Controller::LEFT_TRIGGER)) / (1.0f * 32768);
        }
        else if (trigger == Controller::RIGHT_TRIGGER)
        {
            x = SDL_GameControllerGetAxis(gameController, static_cast<SDL_GameControllerAxis>(Controller::RIGHT_TRIGGER)) / (1.0f * 32768);
        }
    }
    
    return x;
}

bool Input::IsControllerButtonPressed(const int indexID, const Controller::ControllerCode button)
{
    if (m_Controller->GetCount() && !m_Controller->ConfigIsRunning())
    {
        auto gameController = m_Controller->GetGameController(indexID);
        
        return SDL_GameControllerGetButton(gameController, static_cast<SDL_GameControllerButton>(button));
    }
    else
    {
        return false;
    }
}

uint Input::GetControllerCount()
{
    return m_Controller->GetCount();
}

int Input::GetActiveController()
{
    return m_Controller->GetActiveController();
}
