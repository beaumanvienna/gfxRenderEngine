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
   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. 
    
   The code in this file is based on and inspired by the project
   https://github.com/TheCherno/Hazel. The license of this prject can
   be found under https://github.com/TheCherno/Hazel/blob/master/LICENSE
   */

#include "GLframebuffer.h"
#include "GL.h"

static const uint s_MaxFramebufferSize = 8192;

namespace Utils {

    static GLenum TextureTarget(bool multisampled)
    {
        return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
    }

    static void CreateTextures(bool multisampled, uint* outID, uint count)
    {
        GLCall(glCreateTextures(TextureTarget(multisampled), count, outID));
    }

    static void BindTexture(bool multisampled, uint id)
    {
        GLCall(glBindTexture(TextureTarget(multisampled), id));
    }

    static void AttachColorTexture(uint id, int samples, GLenum internalFormat, GLenum format, uint width, uint height, int index)
    {
        bool multisampled = samples > 1;
        if (multisampled)
        {
            GLCall(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE));
        }
        else
        {
            GLCall(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr));

            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        }

        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0));
    }

    static void AttachDepthTexture(uint id, int samples, GLenum format, GLenum attachmentType, uint width, uint height)
    {
        bool multisampled = samples > 1;
        if (multisampled)
        {
            GLCall(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE));
        }
        else
        {
            GLCall(glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height));

            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        }

        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0));
    }

    static bool IsDepthFormat(FramebufferTextureFormat format)
    {
        switch (format)
        {
            case FramebufferTextureFormat::DEPTH24STENCIL8:  return true;
        }

        return false;
    }

    static GLenum FBTextureFormatToGL(FramebufferTextureFormat format)
    {
        switch (format)
        {
            case FramebufferTextureFormat::RGBA8:       return GL_RGBA8;
            case FramebufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
        }

        ASSERT(false);
        return 0;
    }

}

GLFramebuffer::GLFramebuffer(const FramebufferSpecification& spec)
    : m_Specification(spec)
{
    for (auto spec : m_Specification.m_Attachments.m_Attachments)
    {
        if (!Utils::IsDepthFormat(spec.m_TextureFormat))
        {
            m_ColorAttachmentSpecifications.emplace_back(spec);
        }
        else
        {
            m_DepthAttachmentSpecification = spec;
        }
    }

    Recreate();
}

GLFramebuffer::~GLFramebuffer()
{
    GLCall(glDeleteFramebuffers(1, &m_RendererID));
    GLCall(glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data()));
    GLCall(glDeleteTextures(1, &m_DepthAttachment));
}

void GLFramebuffer::Recreate()
{
    if (m_RendererID)
    {
        GLCall(glDeleteFramebuffers(1, &m_RendererID));
        GLCall(glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data()));
        GLCall(glDeleteTextures(1, &m_DepthAttachment));
        
        m_ColorAttachments.clear();
        m_DepthAttachment = 0;
    }

    GLCall(glCreateFramebuffers(1, &m_RendererID));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));

    bool multisample = m_Specification.m_Samples > 1;

    // Attachments
    if (m_ColorAttachmentSpecifications.size())
    {
        m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());
        Utils::CreateTextures(multisample, m_ColorAttachments.data(), m_ColorAttachments.size());

        for (int i = 0; i < m_ColorAttachments.size(); i++)
        {
            Utils::BindTexture(multisample, m_ColorAttachments[i]);
            switch (m_ColorAttachmentSpecifications[i].m_TextureFormat)
            {
                case FramebufferTextureFormat::RGBA8:
                    Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.m_Samples, GL_RGBA8, GL_RGBA, m_Specification.m_Width, m_Specification.m_Height, i);
                    break;
                case FramebufferTextureFormat::RED_INTEGER:
                    Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.m_Samples, GL_R32I, GL_RED_INTEGER, m_Specification.m_Width, m_Specification.m_Height, i);
                    break;
            }
        }
    }

    if (m_DepthAttachmentSpecification.m_TextureFormat != FramebufferTextureFormat::NONE)
    {
        Utils::CreateTextures(multisample, &m_DepthAttachment, 1);
        Utils::BindTexture(multisample, m_DepthAttachment);
        switch (m_DepthAttachmentSpecification.m_TextureFormat)
        {
            case FramebufferTextureFormat::DEPTH24STENCIL8:
                Utils::AttachDepthTexture(m_DepthAttachment, m_Specification.m_Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.m_Width, m_Specification.m_Height);
                break;
        }
    }

    if (m_ColorAttachments.size() > 1)
    {
        ASSERT(m_ColorAttachments.size() <= 4);
        GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
        GLCall(glDrawBuffers(m_ColorAttachments.size(), buffers));
    }
    else if (m_ColorAttachments.empty())
    {
        // Only depth-pass
        GLCall(glDrawBuffer(GL_NONE));
    }

    ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void GLFramebuffer::Bind()
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
    GLCall(glViewport(0, 0, m_Specification.m_Width, m_Specification.m_Height));
}

void GLFramebuffer::Unbind()
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void GLFramebuffer::Resize(uint width, uint height)
{
    if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
    {
        LOG_CORE_WARN("Attempted to rezize framebuffer to {0}, {1}", width, height);
        return;
    }
    m_Specification.m_Width = width;
    m_Specification.m_Height = height;
    
    Recreate();
}

int GLFramebuffer::ReadPixel(uint attachmentIndex, int x, int y)
{
    ASSERT(attachmentIndex < m_ColorAttachments.size());

    GLCall(glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex));
    int pixelData;
    GLCall(glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData));
    return pixelData;

}

void GLFramebuffer::ClearAttachment(uint attachmentIndex, int value)
{
    ASSERT(attachmentIndex < m_ColorAttachments.size());

    auto& spec = m_ColorAttachmentSpecifications[attachmentIndex];
    GLCall(glClearTexImage(m_ColorAttachments[attachmentIndex], 0,
    Utils::FBTextureFormatToGL(spec.m_TextureFormat), GL_INT, &value));
}

uint GLFramebuffer::GetColorAttachmentRendererID(uint index = 0) const
{ 
    ASSERT(index < m_ColorAttachments.size());

    return m_ColorAttachments[index]; 
}
