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

#pragma once

#include <memory>

#include "engine.h"
#include "platform.h"
#include "controllerEvent.h"
#include "glm.hpp"
#include "controller.h"

class Input
{

public:
    static void Start(Controller* controller);

    // keyboard
    static bool IsKeyPressed(const int key);

    // mouse
    static bool IsMouseButtonPressed(const int button);
    static glm::vec2 GetMousePosition();
    static float GetMouseX();
    static float GetMouseY();
    
    // controller
    static void OnUpdate();
    static void ControllerAxisMoved(ControllerAxisMovedEvent event);
    static glm::vec2 GetControllerStick(int indexID, Controller::ControllerSticks stick);

private:

    static Controller* m_Controller;

    static int m_ControllerAxisLeftX[Controller::MAX_NUMBER_OF_CONTROLLERS];
    static int m_ControllerAxisLeftY[Controller::MAX_NUMBER_OF_CONTROLLERS];
    static int m_ControllerAxisLeftXLast[Controller::MAX_NUMBER_OF_CONTROLLERS];
    static int m_ControllerAxisLeftYLast[Controller::MAX_NUMBER_OF_CONTROLLERS];
    static float m_ControllerAxisLeftX_float[Controller::MAX_NUMBER_OF_CONTROLLERS];
    static float m_ControllerAxisLeftY_float[Controller::MAX_NUMBER_OF_CONTROLLERS];
    
    static int m_ControllerAxisRightX[Controller::MAX_NUMBER_OF_CONTROLLERS];
    static int m_ControllerAxisRightY[Controller::MAX_NUMBER_OF_CONTROLLERS];
    static int m_ControllerAxisRightXLast[Controller::MAX_NUMBER_OF_CONTROLLERS];
    static int m_ControllerAxisRightYLast[Controller::MAX_NUMBER_OF_CONTROLLERS];
    static float m_ControllerAxisRightX_float[Controller::MAX_NUMBER_OF_CONTROLLERS];
    static float m_ControllerAxisRightY_float[Controller::MAX_NUMBER_OF_CONTROLLERS];

};
