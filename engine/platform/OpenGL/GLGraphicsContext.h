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
#include "GL.h"
#include "graphicsContext.h"

class GLContext : public GraphicsContext
{
public:

    GLContext(GLFWwindow* window, uint refreshRate);

    virtual bool Init() override;
    virtual void SetVSync(int interval) override;
    virtual void SwapBuffers() override;
    virtual bool IsInitialized() const override { return m_Initialized; }

private:

    bool m_Initialized;
    GLFWwindow* m_Window;
    uint m_RefreshRate; // frames per second
    uint m_MicroSecondsPerFrame; // (in micro seconds)
    
    int m_VSync;
    int m_VSyncIsWorking = 10; // gets decremented if not working
    double m_StartTime;

};
    
