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

#include "background.h"
#include "renderer.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

namespace ScabbApp
{
    void Background::OnAttach() 
    {
        InitAnimation();

        // clouds
        m_CloudSprite = m_SpritesheetMarley->GetSprite(I_CLOUDS);
        m_CloudSprite->SetScale(1.5f, 1.5f);

        // beach
        m_BeachSprite = m_SpritesheetMarley->GetSprite(I_BEACH);
        m_BeachSprite->SetScale(1.5f, 1.5f);
    }

    void Background::InitAnimation()
    {
        // moving clouds
        glm::vec2 cloudsLeft(-1919.0f, 0.0f);
        glm::vec2 cloudsMiddle(0.0f, 0.0f);
        glm::vec2 cloudsRight(1919.0f, 0.0f);

        cloudAnimationRight.AddTranslation(Translation(100.0f, cloudsMiddle, cloudsRight));
        cloudAnimationLeft.AddTranslation(Translation( 100.0f, cloudsLeft,   cloudsMiddle));
    }

    void Background::OnDetach()  {}

    void Background::OnUpdate() 
    {
        static bool mainMessage = true;
        if (mainMessage)
        {
            mainMessage = false;
            LOG_APP_INFO("main screen is running");
        }
        m_SpritesheetMarley->BeginScene();

        // --- clouds ---
        {
            glm::mat4 position;
            if (!cloudAnimationRight.IsRunning()) cloudAnimationRight.Start();
            if (!cloudAnimationLeft.IsRunning()) cloudAnimationLeft.Start();

            // transformed positions
            position = cloudAnimationRight.GetTransformation() * m_CloudSprite->GetScaleMatrix();
            m_Renderer->Draw(m_CloudSprite, position);

            position = cloudAnimationLeft.GetTransformation() * m_CloudSprite->GetScaleMatrix();
            m_Renderer->Draw(m_CloudSprite, position);
        }

        // --- beach ---
        {
            // transformed position
            glm::mat4 position = m_BeachSprite->GetScaleMatrix();
            m_Renderer->Draw(m_BeachSprite, position, -0.05f);
        }
    }

    void Background::OnEvent(Event& event)  {}
}
