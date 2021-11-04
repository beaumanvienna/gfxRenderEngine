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

#include "core.h"
#include "renderer.h"
#include "marley/splash/splash.h"
#include "gtc/matrix_transform.hpp"
#include "resources.h"
#include "glm.hpp"

namespace MarleyApp
{

    void Splash::OnAttach() 
    { 
        m_SpritesheetSplash.AddSpritesheetRow("/images/splashscreen/splash_spritesheet.png", IDB_SPLASH, "PNG", 10 /* frames */, 4.6153846f /* scaleX) */, 4.8f /* scaleY) */);
        m_Splash.Create(200 /* milliseconds per frame */, &m_SpritesheetSplash);

        m_SpritesheetFireworks.AddSpritesheetRow("/images/splashscreen/splash_fireworks.png", IDB_FIREWORKS, "PNG", 20 /* frames */, 4.6153846f /* scaleX) */, 4.8f /* scaleY) */);
        m_SplashFireworks.Create(200 /* milliseconds per frame */, &m_SpritesheetFireworks);
        m_SplashFireworks.Start();

        if (CoreSettings::m_EnableSystemSounds) Engine::m_Engine->PlaySound("/sounds/waves.ogg", IDR_WAVES, "OGG");
        
        m_StartTime =  std::chrono::steady_clock::now();
    }

    void Splash::OnDetach() 
    {

    }

    void Splash::OnUpdate() 
    {
        static bool splashMessage = true;
        if (splashMessage)
        {
            splashMessage = false;
            LOG_APP_INFO("splash is running");
        }
        if (!m_Splash.IsRunning()) m_Splash.Start();

        m_SpritesheetSplash.BeginScene();
        m_SpritesheetFireworks.BeginScene();

        {
            Sprite* sprite = m_Splash.GetSprite();

            // transformed position
            glm::mat4 position = sprite->GetScaleMatrix();

            m_Renderer->Draw(sprite, position);
        }

        if (m_SplashFireworks.IsRunning())
        {
            Sprite* sprite = m_SplashFireworks.GetSprite();

            // transformed position
            glm::mat4 position = sprite->GetScaleMatrix();

            m_Renderer->Draw(sprite, position);
        }

    }

    void Splash::OnEvent(Event& event)  {}
}
