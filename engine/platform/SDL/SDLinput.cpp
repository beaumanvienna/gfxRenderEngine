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

int Input::m_ControllerAxisLeftX[Controller::MAX_NUMBER_OF_CONTROLLERS];
int Input::m_ControllerAxisLeftY[Controller::MAX_NUMBER_OF_CONTROLLERS];
int Input::m_ControllerAxisLeftXLast[Controller::MAX_NUMBER_OF_CONTROLLERS];
int Input::m_ControllerAxisLeftYLast[Controller::MAX_NUMBER_OF_CONTROLLERS];
float Input::m_ControllerAxisLeftX_float[Controller::MAX_NUMBER_OF_CONTROLLERS];
float Input::m_ControllerAxisLeftY_float[Controller::MAX_NUMBER_OF_CONTROLLERS];

int Input::m_ControllerAxisRightX[Controller::MAX_NUMBER_OF_CONTROLLERS];
int Input::m_ControllerAxisRightY[Controller::MAX_NUMBER_OF_CONTROLLERS];
int Input::m_ControllerAxisRightXLast[Controller::MAX_NUMBER_OF_CONTROLLERS];
int Input::m_ControllerAxisRightYLast[Controller::MAX_NUMBER_OF_CONTROLLERS];
float Input::m_ControllerAxisRightX_float[Controller::MAX_NUMBER_OF_CONTROLLERS];
float Input::m_ControllerAxisRightY_float[Controller::MAX_NUMBER_OF_CONTROLLERS];

void Input::Start(Controller* controller)
{
    m_Controller = controller;
    for (int i = 0; i < Controller::MAX_NUMBER_OF_CONTROLLERS; i++)
    {
        m_ControllerAxisLeftX[i] = 0;
        m_ControllerAxisLeftY[i] = 0;
        m_ControllerAxisLeftXLast[i] = 0;
        m_ControllerAxisLeftYLast[i] = 0;
        m_ControllerAxisLeftX_float[i] = 0;
        m_ControllerAxisLeftY_float[i] = 0;
        
        m_ControllerAxisRightX[i] = 0;
        m_ControllerAxisRightY[i] = 0;
        m_ControllerAxisRightXLast[i] = 0;
        m_ControllerAxisRightYLast[i] = 0;
        m_ControllerAxisRightX_float[i] = 0;
        m_ControllerAxisRightY_float[i] = 0;
    }
}

void Input::ControllerAxisMoved(ControllerAxisMovedEvent event)
{
    // get controller
    int indexID = event.GetControllerIndexID();
    
    if (indexID >= Controller::MAX_NUMBER_OF_CONTROLLERS) 
    {
        return;
    }
    
    // left stick
    if (event.GetAxis() == Controller::LEFT_STICK_HORIZONTAL)
    {
        m_ControllerAxisLeftX[indexID] = event.GetAxisValue();
    }
    
    if (event.GetAxis() == Controller::LEFT_STICK_VERTICAL)
    {
        m_ControllerAxisLeftY[indexID] = -event.GetAxisValue();
    }
    
    // right stick
    if (event.GetAxis() == Controller::RIGHT_STICK_HORIZONTAL)
    {
        m_ControllerAxisRightX[indexID] = event.GetAxisValue();
    }
    
    if (event.GetAxis() == Controller::RIGHT_STICK_VERTICAL)
    {
        m_ControllerAxisRightY[indexID] = -event.GetAxisValue();
    }
}

void Input::OnUpdate()
{
    for (int indexID = 0; indexID < m_Controller->GetCount(); indexID++)
    {
        if (abs(m_ControllerAxisLeftX[indexID]) < Controller::ANALOG_DEAD_ZONE)
        {
            // decay
            m_ControllerAxisLeftXLast[indexID] >>= 1;
            m_ControllerAxisLeftX[indexID] = m_ControllerAxisLeftXLast[indexID];
        }
        else
        {
            m_ControllerAxisLeftXLast[indexID] = m_ControllerAxisLeftX[indexID];
        }
        m_ControllerAxisLeftX_float[indexID] = m_ControllerAxisLeftX[indexID] / (1.0f * 32768);
        
        if (abs(m_ControllerAxisLeftY[indexID]) < Controller::ANALOG_DEAD_ZONE)
        {
            // decay
            m_ControllerAxisLeftYLast[indexID] >>= 1;
            m_ControllerAxisLeftY[indexID] = m_ControllerAxisLeftYLast[indexID];
        }
        else
        {
            m_ControllerAxisLeftYLast[indexID] = m_ControllerAxisLeftY[indexID];
        }
        m_ControllerAxisLeftY_float[indexID] = m_ControllerAxisLeftY[indexID] / (1.0f * 32768);
        
        // right
        if (abs(m_ControllerAxisRightX[indexID]) < Controller::ANALOG_DEAD_ZONE)
        {
            // decay
            m_ControllerAxisRightXLast[indexID] >>= 1;
            m_ControllerAxisRightX[indexID] = m_ControllerAxisRightXLast[indexID];
        }
        else
        {
            m_ControllerAxisRightXLast[indexID] = m_ControllerAxisRightX[indexID];
        }
        m_ControllerAxisRightX_float[indexID] = m_ControllerAxisRightX[indexID] / (1.0f * 32768);
        
        if (abs(m_ControllerAxisRightY[indexID]) < Controller::ANALOG_DEAD_ZONE)
        {
            // decay
            m_ControllerAxisRightYLast[indexID] >>= 1;
            m_ControllerAxisRightY[indexID] = m_ControllerAxisRightYLast[indexID];
        }
        else
        {
            m_ControllerAxisRightYLast[indexID] = m_ControllerAxisRightY[indexID];
        }
        m_ControllerAxisRightY_float[indexID] = m_ControllerAxisRightY[indexID] / (1.0f * 32768);
    }
}   

glm::vec2 Input::GetControllerStick(int indexID, Controller::ControllerSticks stick)
{
    if (stick == Controller::LEFT_STICK)
    {
        return { m_ControllerAxisLeftX_float[indexID], m_ControllerAxisLeftY_float[indexID] };
    }
    else if (stick == Controller::RIGHT_STICK)
    {
        return { m_ControllerAxisRightX_float[indexID], m_ControllerAxisRightY_float[indexID] };
    }

    return { 0, 0 };
}
