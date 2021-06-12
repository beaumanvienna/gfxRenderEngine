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

#include <chrono>
#include <thread>

#include "engine.h"
#include "platform.h"
#include "core.h"
#include "engineApp.h"
#include "application.h"
#include "event.h"

const int INVALID_ID = 0;

extern Application application;

int main(int argc, char* argv[])
{  
    Engine engine(argc, argv);
    if (!engine.Start(RendererAPI::OPENGL))
    {
        return -1;
    }
    
    Application application;
    engine.SetAppEventCallback([&](Event& event) { application.OnEvent(event); } );
    
    if (!application.Start())
    {
        return -1;
    }
    
    LOG_CORE_INFO("entering main application");
    while (engine.IsRunning())
    {
        engine.OnUpdate();
        if (!engine.IsPaused())
        {
            application.OnUpdate();
            engine.OnRender();
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::microseconds(100000));
        }
    }

    engine.Shutdown();

    return 0;
};
