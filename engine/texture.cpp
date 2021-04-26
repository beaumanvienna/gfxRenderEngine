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

#include "texture.h"
#include "stb_image.h"
#include "OpenGL/GL.h"

Texture::Texture(const std::string& fileName)
    : m_FileName(fileName), m_RendererID(0), m_LocalBuffer(nullptr), 
      m_Width(0), m_Height(0), m_BPP(0)
{
    int channels_in_file;
    stbi_set_flip_vertically_on_load(true);
    m_LocalBuffer = stbi_load(fileName.c_str(), &m_Width, &m_Height, &m_BPP, 4);
    if(m_LocalBuffer)
    {
        GLCall(glGenTextures(1, &m_RendererID));
        Bind();
        
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
        
        const int BITS_PER_CHANNEL = 8;
        GLCall(glTexImage2D
        (
            GL_TEXTURE_2D,       /* GLenum target,        */
            0,                   /* GLint level,          */
            GL_RGBA,             /* GLint internalformat, */
            m_Width,             /* GLsizei width,        */
            m_Height,            /* GLsizei height,       */
            0,                   /* GLint border,         */
            GL_RGBA,             /* GLenum format,        */
            GL_UNSIGNED_BYTE,    /* GLenum type,          */
            m_LocalBuffer        /* const void * data);   */
        ));
        Unbind();    
        //free local buffer
        stbi_image_free(m_LocalBuffer);
    }
    else
    {
        std::cout << "Texture: Couldn't load file " << fileName << std::endl;
    }
}

Texture::~Texture()
{
    GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(uint slot /* = 0 */) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const
{
    GLCall(glBindTexture(GL_TEXTURE_2D, INVALID_ID));
}
