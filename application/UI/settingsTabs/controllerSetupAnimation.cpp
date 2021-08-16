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

#include "controllerSetupAnimation.h"
#include "renderer.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "resources.h"
#include "matrix.h"

void ControllerSetupAnimation::OnAttach() 
{ 
    m_SetupController = m_SpritesheetMarley->GetSprite(I_PS3_CONTROLLER);
    
    m_SpritesheetPointers.AddSpritesheetRow("/images/images/I_CONTROLLER_SETUP.png", IDB_CONTROLLER_SETUP, "PNG", 19 /* frames */, 1.0f /* scaleX) */, 1.0f /* scaleY) */);
    m_ControllerSetupAnimationPointers.Create(1200 /* milliseconds per frame */, &m_SpritesheetPointers);
}

void ControllerSetupAnimation::OnDetach() 
{
}

void ControllerSetupAnimation::Start()
{
    if (!m_ControllerSetupAnimationPointers.IsRunning())
    {
        m_ControllerSetupAnimationPointers.Start();
    }
}

void ControllerSetupAnimation::OnUpdate() 
{
    m_SpritesheetPointers.BeginScene();

    glm::vec3 translation{0.0f, -300.0f, 0.0f};
    glm::mat4 translationMatrix = Translate(translation);

    // controller picture
    {
        // transformed position
        glm::mat4 position = translationMatrix * m_SetupController->GetScaleMatrix();

        m_Renderer->Draw(m_SetupController, position);
    }

    // arrows
    {
        Sprite* sprite = m_ControllerSetupAnimationPointers.GetSprite();

        // transformed position
        glm::mat4 position = translationMatrix * sprite->GetScaleMatrix();

        m_Renderer->Draw(sprite, position);
    }
}

void ControllerSetupAnimation::OnEvent(Event& event) 
{
}
