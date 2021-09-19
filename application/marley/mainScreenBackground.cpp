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

#include "marley/mainScreenBackground.h"
#include "renderer.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

namespace MarleyApp
{

    bool showTabIcons = false;
    float duration = 1.0f, previousDuration = 1.0f;

    void MainScreenBackground::OnAttach() 
    {
        InitAnimation();

        // clouds
        m_CloudSprite = m_SpritesheetMarley->GetSprite(I_CLOUDS);
        m_CloudSprite->SetScale(1.5f, 1.5f);

        // beach
        m_BeachSprite = m_SpritesheetMarley->GetSprite(I_BEACH);
        m_BeachSprite->SetScale(1.5f, 1.5f);

        // tab
        m_TabSprite = m_SpritesheetMarley->GetSprite(I_TAB_R);
    }

    void MainScreenBackground::InitAnimation()
    {
        // moving clouds
        glm::vec2 cloudsLeft(-1919.0f, 0.0f);
        glm::vec2 cloudsMiddle(0.0f, 0.0f);
        glm::vec2 cloudsRight(1919.0f, 0.0f);

        cloudAnimationRight.AddTranslation(Translation(100.0f * duration, cloudsMiddle, cloudsRight));
        cloudAnimationLeft.AddTranslation(Translation( 100.0f * duration, cloudsLeft,   cloudsMiddle));

        // moving tab
        glm::vec2 finalOutOfScreenPosTab(-1400.0f, 0.0f);
        glm::vec2 finalScreenPosTab(0.0f, 0.0f);

        // tab: move left to center
        tabAnimation.AddTranslation(Translation(1.0f * duration, finalOutOfScreenPosTab, finalScreenPosTab));
        tabAnimation.AddRotation(Rotation(      1.0f * duration, 0.0f, 0.0f));                                              // idle
        tabAnimation.AddScaling(Scaling(        0.9f * duration, 0.6f, 0.6f));
        tabAnimation.AddScaling(Scaling(        0.1f * duration, 0.6f, 1.0f));

        // tab: wiggle
        const float rotationTiming = 0.75f;
        tabAnimation.AddTranslation(Translation(1.0f * duration * rotationTiming, finalScreenPosTab, finalScreenPosTab));   // idle
        tabAnimation.AddRotation(Rotation(      0.1f * duration * rotationTiming,  0.0f,  0.2f));
        tabAnimation.AddRotation(Rotation(      0.2f * duration * rotationTiming,  0.2f, -0.2f));
        tabAnimation.AddRotation(Rotation(      0.2f * duration * rotationTiming, -0.2f,  0.2f));
        tabAnimation.AddRotation(Rotation(      0.2f * duration * rotationTiming,  0.2f, -0.1f));
        tabAnimation.AddRotation(Rotation(      0.2f * duration * rotationTiming, -0.1f,  0.1f));
        tabAnimation.AddRotation(Rotation(      0.1f * duration * rotationTiming,  0.1f,  0.0f));
        tabAnimation.AddScaling(Scaling(        1.0f * duration * rotationTiming,  1.0f,  1.0f));                           // idle

        // tab: idle
        tabAnimation.AddTranslation(Translation(0.5f * duration, finalScreenPosTab, finalScreenPosTab));                    // idle
        tabAnimation.AddRotation(Rotation(      0.4f * duration,  0.0f, 0.0f));                                             // idle
        tabAnimation.AddScaling(Scaling(        0.5f * duration,  1.0f, 1.0f));                                             //idle

        // tab: move center to left
        tabAnimation.AddTranslation(Translation(1.0f * duration, finalScreenPosTab, finalOutOfScreenPosTab));
        tabAnimation.AddRotation(Rotation(      0.1f * duration, -0.05f, 0.0f));
        tabAnimation.AddRotation(Rotation(      0.9f * duration,  0.0f,  0.0f));                                            // idle
        tabAnimation.AddScaling(Scaling(        0.1f * duration,  1.0f,  0.6f));
        tabAnimation.AddScaling(Scaling(        0.9f * duration,  0.6f,  0.6f));
    }

    void MainScreenBackground::OnDetach() 
    {

    }

    void MainScreenBackground::OnUpdate() 
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
 
        // --- third image ---
        if (showTabIcons)
        {

            // model matrix
            if (duration != previousDuration)
            {
                tabAnimation.Stop();
                cloudAnimationRight.Stop();
                cloudAnimationLeft.Stop();
                InitAnimation();
            }
            previousDuration = duration;
            if (!tabAnimation.IsRunning()) tabAnimation.Start();
            auto animationMatrix = tabAnimation.GetTransformation();

            // transformed position
            glm::mat4 position = animationMatrix * m_TabSprite->GetScaleMatrix();
            m_Renderer->Draw(m_TabSprite, position, -0.07f);

        }
    }

    void MainScreenBackground::OnEvent(Event& event)  {}
}
