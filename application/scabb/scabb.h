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

#pragma once

#define APP_INCLUDE 1

#include <memory>
#include <iostream>

#include "engine.h"
#include "application.h"
#include "glm.hpp"
//#include "appSettings.h"
#include "cursor.h"
#include "scabb/background.h"

namespace ScabbApp
{
    class Scabb : public Application
    {

    public:

        virtual bool Start() override;
        virtual void Shutdown() override;
        virtual void OnUpdate() override;
        virtual void OnEvent(Event& event) override;
        virtual void Flush() override;
        virtual std::string GetConfigFilePath() override;

        void OnResize();
        void OnScroll();
        void InitSettings();
        void InitCursor();

        static Scabb* m_Application;

        std::shared_ptr<Cursor> m_Cursor;
        SpriteSheet m_SpritesheetMarley;

    private:

        // layers
        Background*   m_Background = nullptr;

        //AppSettings m_AppSettings{&Engine::m_SettingsManager};
        void CreateConfigFolder();

    };
}
