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

#include "marley/UI/stars.h"
#include "matrix.h"
#include <gtx/transform.hpp>

namespace MarleyApp
{

    void UIStarIcon::OnAttach() 
    {
        float duration = 1.0f;
        // star icon
        m_StarSprite = m_SpritesheetMarley->GetSprite(I_STAR);
        m_StarSprite->SetScale(2.0f);

        // 1st star icon: move right to top right corner
        glm::vec2 finalOutOfScreenPosition;
        glm::vec2 finalScreenPosition1;
        if (m_Narrow)
        {
            finalOutOfScreenPosition= glm::vec2(1150.0f,   0.0f);
            finalScreenPosition1    = glm::vec2( 320.0f, 110.0f);
        }
        else
        {
            finalOutOfScreenPosition= glm::vec2(1150.0f, 260.0f);
            finalScreenPosition1    = glm::vec2( 833.0f, 260.0f);
        }

        m_StarMoveIn1.AddTranslation(Translation(1.0f * duration, finalOutOfScreenPosition, finalScreenPosition1));
        m_StarMoveIn1.AddRotation(Rotation(      1.0f * duration,    0.0f,   3.141f));

        m_StarRotate1.AddRotation(Rotation(      100.0f * duration,    0.0f,  250.0f));
        m_StarRotate1.AddTranslation(Translation(100.0f * duration, finalScreenPosition1, finalScreenPosition1));

        m_StarMoveOut1.AddTranslation(Translation(1.0f * duration, finalScreenPosition1, finalOutOfScreenPosition));
        m_StarMoveOut1.AddRotation(Rotation(      1.0f * duration,    0.0f,   3.141f));

        // 2nd star icon: move left to top left corner
        glm::vec2 finalScreenPosition2;
        if (m_Narrow)
        {
            finalScreenPosition2 = glm::vec2(-310.0f, 120.0f);
        }
        else
        {
            finalScreenPosition2 = glm::vec2(-860.0f, 330.0f);
        }

        m_StarMoveIn2.AddTranslation(Translation(1.0f * duration, finalOutOfScreenPosition, finalScreenPosition2));
        m_StarMoveIn2.AddRotation(Rotation(      1.0f * duration,    0.0f,   -3.141f));

        m_StarRotate2.AddRotation(Rotation(      100.0f * duration,    0.0f,  -250.0f));
        m_StarRotate2.AddTranslation(Translation(100.0f * duration, finalScreenPosition2, finalScreenPosition2));

        m_StarMoveOut2.AddTranslation(Translation(1.0f * duration, finalScreenPosition2, finalOutOfScreenPosition));
        m_StarMoveOut2.AddRotation(Rotation(      1.0f * duration,    0.0f,   3.141f));

        // 3rd star icon: move left to bottom left corner
        glm::vec2 finalScreenPosition3;
        if (m_Narrow)
        {
            finalScreenPosition3 = glm::vec2(-310.0f, -165.0f);
        }
        else
        {
            finalScreenPosition3 = glm::vec2(-530.0f, -280.0f);
        }

        m_StarMoveIn3.AddTranslation(Translation(1.0f * duration, finalOutOfScreenPosition, finalScreenPosition3));
        m_StarMoveIn3.AddRotation(Rotation(      1.0f * duration,    0.0f,   3.141f));

        m_StarRotate3.AddRotation(Rotation(      100.0f * duration,    0.0f,  250.0f));
        m_StarRotate3.AddTranslation(Translation(100.0f * duration, finalScreenPosition3, finalScreenPosition3));

        m_StarMoveOut3.AddTranslation(Translation(1.0f * duration, finalScreenPosition3, finalOutOfScreenPosition));
        m_StarMoveOut3.AddRotation(Rotation(      1.0f * duration,    0.0f,   3.141f));

        m_Running = false;
        m_Start   = false;
        m_Stop    = false;

    }

    void UIStarIcon::OnDetach() 
    {
    }

    void UIStarIcon::OnUpdate()
    {
        if (!m_StarMoveIn1.IsRunning() && !m_Running && m_Start)
        {
            m_StarMoveIn1.Start();
            m_StarMoveIn2.Start();
            m_StarMoveIn3.Start();
            m_Running = true;
            m_Stop    = false;
        }
        else if (m_Running && !m_Stop)
        {
            if (!m_StarRotate1.IsRunning()) m_StarRotate1.Start();
            if (!m_StarRotate2.IsRunning()) m_StarRotate2.Start();
            if (!m_StarRotate3.IsRunning()) m_StarRotate3.Start();
        }
        else if (m_Running && m_Stop)
        {
            m_StarRotate1.Stop();
            m_StarRotate2.Stop();
            m_StarRotate3.Stop();

            m_StarMoveOut1.Start();
            m_StarMoveOut2.Start();
            m_StarMoveOut3.Start();

            m_Running = false;
            m_Start   = false;
        }
        if (m_StarMoveIn1.IsRunning())
        {
            {
                auto animationMatrix = m_StarMoveIn1.GetTransformation();

                // transformed position
                glm::mat4 position = animationMatrix * m_StarSprite->GetScaleMatrix();
                m_Renderer->Draw(m_StarSprite, position, -0.07f);
            }
            {
                auto animationMatrix = m_StarMoveIn2.GetTransformation();

                // transformed position
                glm::mat4 position = animationMatrix * m_StarSprite->GetScaleMatrix();
                m_Renderer->Draw(m_StarSprite, position, -0.07f);
            }
            {
                auto animationMatrix = m_StarMoveIn3.GetTransformation();

                // transformed position
                glm::mat4 position = animationMatrix * m_StarSprite->GetScaleMatrix();
                m_Renderer->Draw(m_StarSprite, position, -0.07f);
            }
        }
        else if (m_StarRotate1.IsRunning())
        {
            {
                auto animationMatrix = m_StarRotate1.GetTransformation();

                // transformed position
                glm::mat4 position = animationMatrix * m_StarSprite->GetScaleMatrix();
                m_Renderer->Draw(m_StarSprite, position, -0.07f);
            }
            {
                auto animationMatrix = m_StarRotate2.GetTransformation();

                // transformed position
                glm::mat4 position = animationMatrix * m_StarSprite->GetScaleMatrix();
                m_Renderer->Draw(m_StarSprite, position, -0.07f);
            }
            {
                auto animationMatrix = m_StarRotate3.GetTransformation();

                // transformed position
                glm::mat4 position = animationMatrix * m_StarSprite->GetScaleMatrix();
                m_Renderer->Draw(m_StarSprite, position, -0.07f);
            }
        }
        else if (m_StarMoveOut1.IsRunning())
        {
            {
                auto animationMatrix = m_StarMoveOut1.GetTransformation();

                // transformed position
                glm::mat4 position = animationMatrix * m_StarSprite->GetScaleMatrix();
                m_Renderer->Draw(m_StarSprite, position, -0.07f);
            }
            {
                auto animationMatrix = m_StarMoveOut2.GetTransformation();

                // transformed position
                glm::mat4 position = animationMatrix * m_StarSprite->GetScaleMatrix();
                m_Renderer->Draw(m_StarSprite, position, -0.07f);
            }
            {
                auto animationMatrix = m_StarMoveOut3.GetTransformation();

                // transformed position
                glm::mat4 position = animationMatrix * m_StarSprite->GetScaleMatrix();
                m_Renderer->Draw(m_StarSprite, position, -0.07f);
            }
        }
    }

    void UIStarIcon::OnEvent(Event& event) 
    {
    }
}
