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

#include "marley/UI/messageBoard.h"
#include "matrix.h"
#include <gtx/transform.hpp>
#include "marley/UI/settingsScreen.h"

namespace MarleyApp
{

    void MessageBoard::OnAttach() 
    {
        float duration = 1.0f;

        m_MessageBoardSprite = m_SpritesheetMarley->GetSprite(I_SIGN_R);

        // move right to lower right corner
        glm::vec2 finalOutOfScreenPosition(1280.0f, 0.0f);
        glm::vec2 finalScreenPosition(0.0f, 0.0f);

        // move left to center
        m_MessageBoardMoveIn.AddTranslation(Translation(1.0f * duration, finalOutOfScreenPosition, finalScreenPosition));
        m_MessageBoardMoveIn.AddRotation(Rotation(      1.0f * duration,    0.0f,   0.0f));                                           // idle
        m_MessageBoardMoveIn.AddScaling(Scaling(        0.9f * duration,    1.0f,   0.6f,   1.0f,   0.6f));
        m_MessageBoardMoveIn.AddScaling(Scaling(        0.1f * duration,    1.0f,   0.6f,   1.0f,   1.0f));

        // wiggle
        const float rotationTiming = 0.75f;
        m_MessageBoardMoveIn.AddTranslation(Translation(1.0f * duration * rotationTiming, finalScreenPosition, finalScreenPosition)); // idle
        m_MessageBoardMoveIn.AddRotation(Rotation(      0.1f * duration * rotationTiming,   0.0f,   0.2f));
        m_MessageBoardMoveIn.AddRotation(Rotation(      0.2f * duration * rotationTiming,   0.2f,  -0.2f));
        m_MessageBoardMoveIn.AddRotation(Rotation(      0.2f * duration * rotationTiming,  -0.2f,   0.2f));
        m_MessageBoardMoveIn.AddRotation(Rotation(      0.2f * duration * rotationTiming,   0.2f,  -0.1f));
        m_MessageBoardMoveIn.AddRotation(Rotation(      0.2f * duration * rotationTiming,  -0.1f,   0.1f));
        m_MessageBoardMoveIn.AddRotation(Rotation(      0.1f * duration * rotationTiming,   0.1f,   -0.014f));
        m_MessageBoardMoveIn.AddScaling(Scaling(        1.0f * duration * rotationTiming,   1.0f,   1.0f));                           // idle

        // move center to right
        m_MessageBoardMoveOut.AddTranslation(Translation(1.0f * duration, finalScreenPosition, finalOutOfScreenPosition));
        m_MessageBoardMoveOut.AddRotation(Rotation(      0.1f * duration, -0.05f,    0.0f));
        m_MessageBoardMoveOut.AddRotation(Rotation(      0.9f * duration,   0.0f,    0.0f));                                          // idle
        m_MessageBoardMoveOut.AddScaling(Scaling(        0.1f * duration,   1.0f,   1.0f,   1.0f,   0.6f));
        m_MessageBoardMoveOut.AddScaling(Scaling(        0.9f * duration,   1.0f,   0.6f,   1.0f,   0.6f));

        m_Running = false;
        m_Start   = false;
        m_Stop    = false;

    }

    void MessageBoard::OnDetach() 
    {
    }

    void MessageBoard::OnUpdate()
    {
        if (!m_MessageBoardMoveIn.IsRunning() && !m_Running && m_Start)
        {
            m_MessageBoardMoveIn.Start();
            m_Running = true;
            m_Stop    = false;
        }
        else if (m_Running && m_Stop)
        {
            m_MessageBoardMoveOut.Start();

            m_Running = false;
            m_Start   = false;
        }
        if (m_Running)
        {
            {
                auto animationMatrix = m_MessageBoardMoveIn.GetTransformation();

                // transformed position
                glm::mat4 position = animationMatrix * m_MessageBoardSprite->GetScaleMatrix();
                m_Renderer->Draw(m_MessageBoardSprite, position, -0.07f);
            }
        }
        else if (m_MessageBoardMoveOut.IsRunning())
        {
            {
                auto animationMatrix = m_MessageBoardMoveOut.GetTransformation();

                // transformed position
                glm::mat4 position = animationMatrix * m_MessageBoardSprite->GetScaleMatrix();
                m_Renderer->Draw(m_MessageBoardSprite, position, -0.07f);
            }
        }
    }

    void MessageBoard::OnEvent(Event& event) 
    {
    }
}
