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

#pragma once

#include "engine.h"
#include <memory>

enum class FramebufferTextureFormat
{
    NONE = 0,

    // Color
    RGBA8,
    RED_INTEGER,

    // Depth/stencil
    DEPTH24STENCIL8,

    // Defaults
    DEPTH = DEPTH24STENCIL8
};

struct FramebufferTextureSpecification
{
    FramebufferTextureSpecification() = default;
    FramebufferTextureSpecification(FramebufferTextureFormat format)
        : m_TextureFormat(format) {}

    FramebufferTextureFormat m_TextureFormat = FramebufferTextureFormat::NONE;

};

struct FramebufferAttachmentSpecification
{
    FramebufferAttachmentSpecification() = default;
    FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
        : m_Attachments(attachments) {}

    std::vector<FramebufferTextureSpecification> m_Attachments;
};

struct FramebufferSpecification
{
    uint m_Width = 0, m_Height = 0;
    FramebufferAttachmentSpecification m_Attachments;
    uint m_Samples = 1;

    bool m_SwapChainTarget = false;
};

class Framebuffer
{

public:

    virtual ~Framebuffer() = default;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    virtual void Resize(uint width, uint height) = 0;
    virtual int ReadPixel(uint attachmentIndex, int x, int y) = 0;
    virtual void ClearAttachment(uint attachmentIndex, int value) = 0;
    virtual uint GetColorAttachmentRendererID(uint index = 0) const = 0;
    virtual const FramebufferSpecification& GetSpecification() const = 0;

    static std::shared_ptr<Framebuffer> Create(const FramebufferSpecification& spec);
};



