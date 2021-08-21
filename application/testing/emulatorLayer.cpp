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

#include "emulatorLayer.h"
#include "GL.h"
#include "core.h"
#include "glm.hpp"
#include "matrix.h"
#include "resources.h"
#include "renderCommand.h"

void EmulatorLayer::OnAttach() 
{

    FramebufferTextureSpecification textureSpec(FramebufferTextureFormat::RGBA8);
    FramebufferAttachmentSpecification fbAttachments{textureSpec};
    m_FbSpec = FramebufferSpecification {476, 485, fbAttachments, 1, false};
    m_Framebuffer = Framebuffer::Create(m_FbSpec);

    m_FramebufferTexture = ResourceSystem::GetTextureFromMemory("/images/images/I_BARREL.png", IDB_BARREL, "PNG");
    m_FramebufferSprite = new Sprite(0.0f, 1.0f, 1.0f, 0.0f, m_FramebufferTexture->GetWidth(), m_FramebufferTexture->GetHeight(), m_FramebufferTexture, "framebuffer texture", 0.25f, 0.25f);

}

void EmulatorLayer::OnDetach() 
{
    if (m_FramebufferSprite) delete m_FramebufferSprite;
}

void EmulatorLayer::OnUpdate()
{

    m_Framebuffer->Bind();

    static float blue = 0.5f;
    static float delta = 0.01f;
    blue += delta;
    if ((blue < 0.5f) || (blue > 1.0f))
    {
        delta = -delta;
        blue += delta;
    }

    RenderCommand::SetClearColor(glm::vec4(0.5f, 0.2f, blue, 0.5f));
    RenderCommand::Clear();

    m_Framebuffer->Unbind();
    GLCall(glViewport(0, 0, Engine::m_Engine->GetWindowWidth(), Engine::m_Engine->GetWindowHeight()));
    
    m_FramebufferTexture->Bind();

    glm::vec3 translation{-500.0f, 200.0f, 0.0f};

    glm::mat4 position = Translate(translation) * m_FramebufferSprite->GetScaleMatrix();
    m_Renderer->Draw(m_FramebufferSprite, position);

}

void EmulatorLayer::OnEvent(Event& event) 
{
}

