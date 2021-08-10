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

#include "GLcursor.h"
#include "stb_image.h"
#include "core.h"

GLCursor::GLCursor()
    : m_Width(0), m_Height(0), m_BitsPerPixel(0),
      m_HotX(0), m_HotY(0), m_Pixels(nullptr),
      m_Cursor(nullptr), m_Window(nullptr)
{
}

GLCursor::~GLCursor()
{
}

// create cursor internal
bool GLCursor::SetCursor()
{
    bool ok = false;

    if(m_Pixels)
    {
        ok = true;
        GLFWimage image;
        image.width  = m_Width;
        image.height = m_Height;
        image.pixels = m_Pixels;
         
        m_Cursor = glfwCreateCursor(&image, m_HotX, m_HotY);
        m_Window = (GLFWwindow*)Engine::m_Engine->GetWindow();
        glfwSetCursor(m_Window, m_Cursor);
    }

    return ok;
}

// create cursor from file on disk
bool GLCursor::SetCursor(const std::string& fileName, uint xHot, uint yHot)
{
    m_HotX = xHot;
    m_HotY = yHot;

    m_Pixels = stbi_load(fileName.c_str(), &m_Width, &m_Height, &m_BitsPerPixel, 4);

    return SetCursor();
}

// create cursor from file in memory
bool GLCursor::SetCursor(const unsigned char* data, int length, uint xHot, uint yHot)
{
    m_HotX = xHot;
    m_HotY = yHot;

    m_Pixels = stbi_load_from_memory(data, length, &m_Width, &m_Height, &m_BitsPerPixel, 4);

    return SetCursor();
}

void GLCursor::RestorCursor()
{
    if(m_Cursor) glfwSetCursor(m_Window, m_Cursor);
}
