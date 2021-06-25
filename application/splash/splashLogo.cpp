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

#include "splashLogo.h"
#include "matrix.h"
#include <gtx/transform.hpp>
#include "input.h"

extern float duration;

void SplashLogo::OnAttach() 
{
    // logo icon
    m_LogoSprite = m_SpritesheetMarley->GetSprite(0, I_ENGINE);
    m_BannerSprite = m_SpritesheetMarley->GetSprite(0, I_MARLEYLOGO);
    
    glm::vec2 startPositionBanner(0.0f, 0.0f);
    glm::vec2 finalPositionBanner(0.0f, 200.0f);
    float finalScaleBanner = 1.0f;
    
    // logo icon: idle
    m_SplashBanner.AddTranslation(Translation(1.5f * duration, finalPositionBanner, finalPositionBanner));                 // idle
    m_SplashBanner.AddRotation(Rotation(      1.5f * duration,   0.0f,   0.0f));                                           // idle
    m_SplashBanner.AddScaling(Scaling(        1.5f * duration,   0.0f,   0.0f,   0.0f,   0.0f));                           // idle
    
    // logo icon: move into center
    m_SplashBanner.AddTranslation(Translation(0.25f * duration, startPositionBanner, finalPositionBanner));
    m_SplashBanner.AddRotation(Rotation(      0.25f * duration,   0.0f,   0.0f));                                           // idle
    m_SplashBanner.AddScaling(Scaling(        0.25f * duration,   0.0f,   0.0f,   finalScaleBanner,   finalScaleBanner));

    // logo icon: idle
    m_SplashBanner.AddTranslation(Translation(3.25f * duration, finalPositionBanner, finalPositionBanner));                 // idle
    m_SplashBanner.AddRotation(Rotation(      3.25f * duration,   0.0f,   0.0f));                                           // idle
    m_SplashBanner.AddScaling(Scaling(        3.25f * duration,   finalScaleBanner,   finalScaleBanner,   finalScaleBanner,   finalScaleBanner));   // idle
    
    
    glm::vec2 startPosition(0.0f, 0.0f);
    glm::vec2 finalPosition(0.0f, 0.0f);
    float finalScale = 2.0f;
    
    // logo icon: idle
    m_SplashLogo.AddTranslation(Translation(0.5f * duration, finalPosition, finalPosition));                 // idle
    m_SplashLogo.AddRotation(Rotation(      0.5f * duration,   0.0f,   0.0f));                                           // idle
    m_SplashLogo.AddScaling(Scaling(        0.5f * duration,   0.0f,   0.0f,   0.0f,   0.0f));                           // idle
    
    // logo icon: move into center
    m_SplashLogo.AddTranslation(Translation(0.25f * duration, startPosition, finalPosition));
    m_SplashLogo.AddRotation(Rotation(      0.25f * duration,   0.0f,   0.0f));                                           // idle
    m_SplashLogo.AddScaling(Scaling(        0.25f * duration,   0.0f,   0.0f,   finalScale,   finalScale));

    // logo icon: idle
    m_SplashLogo.AddTranslation(Translation(2.25f * duration, finalPosition, finalPosition));                 // idle
    m_SplashLogo.AddRotation(Rotation(      2.25f * duration,   0.0f,   0.0f));                                           // idle
    m_SplashLogo.AddScaling(Scaling(        2.25f * duration,   finalScale,   finalScale,   finalScale,   finalScale));   // idle
    
    // logo icon: expand
    m_SplashLogo.AddTranslation(Translation(1.0f * duration, finalPosition, startPosition));
    m_SplashLogo.AddRotation(Rotation(      1.0f * duration,   0.0f,    0.0f));                                          // idle
    m_SplashLogo.AddScaling(Scaling(      0.125f * duration,   finalScale,   finalScale,   finalScale*1.2f,   finalScale*1.2f));
    m_SplashLogo.AddScaling(Scaling(      0.125f * duration,   finalScale*1.2f,   finalScale*1.2f,   finalScale,   finalScale));
    m_SplashLogo.AddScaling(Scaling(      0.125f * duration,   finalScale,   finalScale,   finalScale*1.2f,   finalScale*1.2f));
    m_SplashLogo.AddScaling(Scaling(      0.125f * duration,   finalScale*1.2f,   finalScale*1.2f,   finalScale,   finalScale));
    m_SplashLogo.AddScaling(Scaling(      0.125f * duration,   finalScale,   finalScale,   finalScale*1.2f,   finalScale*1.2f));
    m_SplashLogo.AddScaling(Scaling(      0.125f * duration,   finalScale*1.2f,   finalScale*1.2f,   finalScale,   finalScale));
    m_SplashLogo.AddScaling(Scaling(      0.125f * duration,   finalScale,   finalScale,   finalScale*1.2f,   finalScale*1.2f));
    m_SplashLogo.AddScaling(Scaling(      0.125f * duration,   finalScale*1.2f,   finalScale*1.2f,   finalScale,   finalScale));
    
    // logo icon: idle
    m_SplashLogo.AddTranslation(Translation(1.0f * duration, finalPosition, finalPosition));                 // idle
    m_SplashLogo.AddRotation(Rotation(      1.0f * duration,   0.0f,   0.0f));                                           // idle
    m_SplashLogo.AddScaling(Scaling(        1.0f * duration,   finalScale,   finalScale,   finalScale,   finalScale));   // idle
    
    m_SplashLogo.Start();
    m_SplashBanner.Start();
}

void SplashLogo::OnDetach() 
{
}

void SplashLogo::OnUpdate()
{    
    if (m_SplashLogo.IsRunning())
    {
        auto logoAnimationMatrix = m_SplashLogo.GetTransformation();
        
        // transformed position
        glm::mat4 position = logoAnimationMatrix * m_LogoSprite->GetScaleMatrix();
        m_Renderer->Draw(m_LogoSprite, position, -0.08f);
        
        auto bannerAnimationMatrix = m_SplashBanner.GetTransformation();
        
        // transformed position
        position = bannerAnimationMatrix * m_BannerSprite->GetScaleMatrix();
        m_Renderer->Draw(m_BannerSprite, position, -0.07f);
    }
}

void SplashLogo::OnEvent(Event& event) 
{
}


