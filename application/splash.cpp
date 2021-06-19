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

#include "splash.h"
#include "renderer.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

void Splash::OnAttach() 
{ 
    m_SpritesheetSplash.AddSpritesheetAnimation("resources/splashscreen/splash_spritesheet2.png", 20 /* frames */, 200 /* milliseconds per frame */, 4.6153846f /* scaleX) */, 4.8f /* scaleY) */);
    m_Splash = m_SpritesheetSplash.GetSpriteAnimation();
    m_Splash->Start();
}

void Splash::OnDetach() 
{
    
}

void Splash::OnUpdate() 
{
    if (m_Splash->IsRunning()) 
    {
        static bool splashMessage = true;
        if (splashMessage)
        {
            splashMessage = false;
            LOG_APP_INFO("splash is running");
        }

        m_SpritesheetSplash.BeginScene();
        {
        
            Sprite* sprite = m_Splash->GetSprite();

            // transformed position
            glm::mat4 position = sprite->GetScaleMatrix();
            
            m_Renderer->Draw(sprite, position, m_SpritesheetSplash.GetTextureSlot());

        }
    }
    else
    {
        m_Splash->Start();
    }
    
}

void Splash::OnEvent(Event& event) 
{
}
