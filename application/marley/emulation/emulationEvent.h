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

#include "marley/appEvent.h"

namespace MarleyApp
{
    class EmulatorLaunchEvent : public AppEvent
    {

    public:

        EmulatorLaunchEvent(const std::string& gameFilename)
            : m_GameFilename(gameFilename) {}

        inline const std::string& GetGameFilename() const { return m_GameFilename; }

        EVENT_CLASS_CATEGORY(EventCategoryApplication);
        EVENT_CLASS_TYPE(ApplicationEvent);
        EVENT_CLASS_APP_CATEGORY(EventCategoryEmulation);
        EVENT_CLASS_APP_TYPE(EmulatorLaunch);

        std::string ToString() const override
        {
            std::stringstream str;
            str << "EmulatorLaunchEvent: game filename: " << m_GameFilename;
            return str.str();
        }

    private:

        std::string m_GameFilename;

    };

    class EmulatorQuitEvent : public AppEvent
    {

    public:

        EmulatorQuitEvent() {}

        EVENT_CLASS_CATEGORY(EventCategoryApplication);
        EVENT_CLASS_TYPE(ApplicationEvent);
        EVENT_CLASS_APP_CATEGORY(EventCategoryEmulation);
        EVENT_CLASS_APP_TYPE(EmulatorQuit);

        std::string ToString() const override
        {
            std::stringstream str;
            str << "EmulatorQuitEvent";
            return str.str();
        }
    };

    class EmulatorSaveEvent : public AppEvent
    {

    public:

        EmulatorSaveEvent() {}

        EVENT_CLASS_CATEGORY(EventCategoryApplication);
        EVENT_CLASS_TYPE(ApplicationEvent);
        EVENT_CLASS_APP_CATEGORY(EventCategoryEmulation);
        EVENT_CLASS_APP_TYPE(EmulatorSave);

        std::string ToString() const override
        {
            std::stringstream str;
            str << "EmulatorSaveEvent";
            return str.str();
        }
    };

    class EmulatorLoadEvent : public AppEvent
    {

    public:

        EmulatorLoadEvent() {}

        EVENT_CLASS_CATEGORY(EventCategoryApplication);
        EVENT_CLASS_TYPE(ApplicationEvent);
        EVENT_CLASS_APP_CATEGORY(EventCategoryEmulation);
        EVENT_CLASS_APP_TYPE(EmulatorLoad);

        std::string ToString() const override
        {
            std::stringstream str;
            str << "EmulatorLoadEvent";
            return str.str();
        }
    };
}
