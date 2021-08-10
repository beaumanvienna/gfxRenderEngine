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

#include "GLtexture.h"
#include "stb_image.h"
#include "log.h"
#include <string>

uint GLTexture::m_TextureSlotCounter = 0;

GLTexture::GLTexture()
    : m_FileName(""), m_RendererID(0), m_LocalBuffer(nullptr), 
      m_Width(0), m_Height(0), m_BPP(0), m_InternalFormat(0), m_DataFormat(0)
{
}

GLTexture::~GLTexture()
{
    GLCall(glDeleteTextures(1, &m_RendererID));
}

// create texture from raw memory
bool GLTexture::Init(const uint width, const uint height, const void* data)
{
    bool ok = false;
    
    m_LocalBuffer = (uchar*)data;
    if(m_LocalBuffer)
    {
        ok = true;
        m_Width = width;
        m_Height = height;
        m_TextureSlot = m_TextureSlotCounter;
        m_TextureSlotCounter++;
        GLCall(glGenTextures(1, &m_RendererID));
        Bind();

        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

        m_BPP = 4;
        m_InternalFormat = GL_RGBA8;
        m_DataFormat = GL_RGBA;

        const int BITS_PER_CHANNEL = 8;
        GLCall(glTexImage2D
        (
            GL_TEXTURE_2D,       /* GLenum target,        */
            0,                   /* GLint level,          */
            m_InternalFormat,    /* GLint internalformat, */
            m_Width,             /* GLsizei width,        */
            m_Height,            /* GLsizei height,       */
            0,                   /* GLint border,         */
            m_DataFormat,        /* GLenum format,        */
            GL_UNSIGNED_BYTE,    /* GLenum type,          */
            m_LocalBuffer        /* const void * data);   */
        ));
        Unbind();
    }

    return ok;
}

// create texture from file on disk
bool GLTexture::Init(const std::string& fileName)
{
    bool ok = false;
    int channels_in_file;
    stbi_set_flip_vertically_on_load(true);
    m_FileName = fileName;
    m_LocalBuffer = stbi_load(m_FileName.c_str(), &m_Width, &m_Height, &m_BPP, 4);
    if(m_LocalBuffer)
    {
        ok = Create();
    }
    else
    {
        std::cout << "Texture: Couldn't load file " << m_FileName << std::endl;
    }
    return ok;
}

// create texture from file in memory
bool GLTexture::Init(const unsigned char* data, int length)
{
    bool ok = false;
    int channels_in_file;
    stbi_set_flip_vertically_on_load(true);

    m_LocalBuffer = stbi_load_from_memory(data, length, &m_Width, &m_Height, &m_BPP, 4);
    
    if(m_LocalBuffer)
    {
        ok = Create();
    }
    else
    {
        std::cout << "Texture: Couldn't load file " << m_FileName << std::endl;
    }
    return ok;
}

bool GLTexture::Create()
{
    m_TextureSlot = m_TextureSlotCounter;
    m_TextureSlotCounter++;
    GLCall(glGenTextures(1, &m_RendererID));
    Bind();
    
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
    
    GLenum internalFormat = 0, dataFormat = 0;
    if (m_BPP == 4)
    {
        internalFormat = GL_RGBA8;
        dataFormat = GL_RGBA;
    }
    else if (m_BPP == 3)
    {
        internalFormat = GL_RGB8;
        dataFormat = GL_RGBA; // GL_RGB did not work with resources/splashscreen/splash_spritesheet2.png
    }
    else
    {
        LOG_CORE_CRITICAL("data format for {0} not supported", m_FileName);
    }
    ASSERT(internalFormat && dataFormat);

    m_InternalFormat = internalFormat;
    m_DataFormat = dataFormat;
    
    const int BITS_PER_CHANNEL = 8;
    GLCall(glTexImage2D
    (
        GL_TEXTURE_2D,       /* GLenum target,        */
        0,                   /* GLint level,          */
        m_InternalFormat,    /* GLint internalformat, */
        m_Width,             /* GLsizei width,        */
        m_Height,            /* GLsizei height,       */
        0,                   /* GLint border,         */
        m_DataFormat,        /* GLenum format,        */
        GL_UNSIGNED_BYTE,    /* GLenum type,          */
        m_LocalBuffer        /* const void * data);   */
    ));
    Unbind();    
    //free local buffer
    stbi_image_free(m_LocalBuffer);
    return true;
}

void GLTexture::Bind() const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + m_TextureSlot));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void GLTexture::Unbind() const
{
    GLCall(glBindTexture(GL_TEXTURE_2D, INVALID_ID));
}
