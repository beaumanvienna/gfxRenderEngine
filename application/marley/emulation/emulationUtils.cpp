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

#include "core.h"
#include "marley/marley.h"
#include "marley/emulation/emulationUtils.h"

namespace MarleyApp
{
    EmulationUtils::EmulationUtils()
    {
        m_ConfigFolder = Engine::m_Engine->GetConfigFilePath();
    }
    void EmulationUtils::CreateConfigFolder()
    {
        if (!EngineCore::IsDirectory(m_ConfigFolder))
        {
            if (EngineCore::CreateDirectory(m_ConfigFolder))
            {
                LOG_APP_INFO("Configuration folder {0} created", m_ConfigFolder);
            }
            else
            {
                LOG_APP_CRITICAL("Couldn't create configuration folder {0}", m_ConfigFolder);
            }
        }
    }
}
