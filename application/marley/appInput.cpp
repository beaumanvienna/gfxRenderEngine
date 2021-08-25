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

#include "marley/appInput.h"
#include "input.h"

namespace MarleyApp
{

    glm::vec2 AppInput::m_MovementInput = glm::vec2(0.0f, 0.0f);

    glm::vec2& AppInput::GetMovementInput()
    {

        if(Input::IsKeyPressed(ENGINE_KEY_D))
        {
            m_MovementInput = glm::vec2( 1.0f, 0.0f);
        }
        else if(Input::IsKeyPressed(ENGINE_KEY_A))
        {
            m_MovementInput = glm::vec2(-1.0f, 0.0f);
        }
        else if(Input::IsKeyPressed(ENGINE_KEY_W))
        {
            m_MovementInput = glm::vec2( 0.0f, 1.0f);
        }
        else if(Input::IsKeyPressed(ENGINE_KEY_S))
        {
            m_MovementInput = glm::vec2( 0.0f,-1.0f);
        }
        else
        {
            m_MovementInput = Input::GetControllerStick(Controller::FIRST_CONTROLLER, Controller::LEFT_STICK);
        }

        return m_MovementInput;

    }
}
