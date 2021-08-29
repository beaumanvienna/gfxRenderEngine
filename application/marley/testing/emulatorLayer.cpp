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

#include "marley/testing/emulatorLayer.h"
#include "GL.h"
#include "core.h"
#include "glm.hpp"
#include "matrix.h"
#include "resources.h"
#include "renderCommand.h"
#include "stb_image.h"

namespace MarleyApp
{

    void EmulatorLayer::OnAttach() 
    {

        FramebufferTextureSpecification textureSpec(FramebufferTextureFormat::RGBA8);
        FramebufferAttachmentSpecification fbAttachments{textureSpec};

        // frame buffer
        m_FbSpec = FramebufferSpecification {256, 224, fbAttachments, 1, false};
        m_Framebuffer = Framebuffer::Create(m_FbSpec);

        // framebuffer texture
        m_FramebufferTexture = Texture::Create();
        m_FramebufferTexture->Init(m_FbSpec.m_Width, m_FbSpec.m_Height, m_Framebuffer->GetColorAttachmentRendererID(0));

        // framebuffer sprite
        m_FramebufferSprite = new Sprite(0.0f, 1.0f, 1.0f, 0.0f, m_FramebufferTexture->GetWidth(), m_FramebufferTexture->GetHeight(), m_FramebufferTexture, "framebuffer texture", 1.0f, 1.0f);

        size_t fileSize;
        const uchar* buffer = (const uchar*)ResourceSystem::GetDataPointer(fileSize, "/images/images/I_DK.png", IDB_DK, "PNG");
        
        m_Pixels = stbi_load_from_memory(buffer, fileSize, &m_Width, &m_Height, &m_BPP, 4);
    }

    void EmulatorLayer::OnDetach() 
    {
        if (m_FramebufferSprite) delete m_FramebufferSprite;
    }

    void EmulatorLayer::BeginScene()
    {
        m_Framebuffer->Bind();
    }

    void EmulatorLayer::EndScene()
    {
        m_Framebuffer->Unbind();
        GLCall(glViewport(0, 0, Engine::m_Engine->GetWindowWidth(), Engine::m_Engine->GetWindowHeight()));
    }

    void EmulatorLayer::OnUpdate()
    {
        
        uint x = 0;
        uint y = 0;

        m_FramebufferTexture->Blit(x, y, m_Width, m_Height, m_BPP, m_Pixels);

        // render frame buffer
        {
            m_FramebufferTexture->Bind();
            glm::vec3 translation{-500.0f, 200.0f, 0.0f};

            glm::mat4 position = Translate(translation) * m_FramebufferSprite->GetScaleMatrix();
            m_Renderer->Draw(m_FramebufferSprite, position);
        }
    }

    void EmulatorLayer::OnEvent(Event& event) 
    {
    }
}
