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

#include "event.h"

namespace MarleyApp
{
    class AppEvent;
    typedef std::function<void(AppEvent&)> AppEventCallbackFunction;
    enum class AppEventType
    {
        None = 0,
        EmulatorLaunch,
        EmulatorPause,
        EmulatorQuit,
        EmulatorLoad,
        EmulatorSave,
        GameStateChanged,
        InputIdle,
        BiosNotFound,
        SceneChanged
    };

    enum AppEventCategory
    {
        None = 0,
        EventCategoryEmulation        = BIT(0),
        EventCategoryGameState        = BIT(1),
        EventCategoryInput            = BIT(2),
    };

    #define EVENT_CLASS_APP_CATEGORY(x) int GetAppCategoryFlags() const override { return x; }
    #define EVENT_CLASS_APP_TYPE(x) static AppEventType GetStaticAppType() { return AppEventType::x; }\
            AppEventType GetAppEventType() const override { return GetStaticAppType(); }\

    class AppEvent : public Event
    {
        friend class AppEventDispatcher;
        virtual AppEventType GetAppEventType() const = 0;
        virtual int GetAppCategoryFlags() const = 0;
    };

    class AppEventDispatcher
    {
        template <typename T>
        using EventFn = std::function<bool(T&)>;

    public:
        AppEventDispatcher(AppEvent& event)
            : m_Event(event) {}

        template<typename T>
        bool Dispatch(EventFn<T> func)
        {
            if (m_Event.GetAppEventType() == T::GetStaticAppType())
            {
                m_Event.m_Handled |= func(*(T*)&m_Event);
                return true;
            }
            return false;
        }

    private:

        AppEvent& m_Event;

    };

    class SceneChangedEvent : public AppEvent
    {

    public:

        SceneChangedEvent() {}

        EVENT_CLASS_CATEGORY(EventCategoryApplication);
        EVENT_CLASS_TYPE(ApplicationEvent);
        EVENT_CLASS_APP_CATEGORY(EventCategoryGameState);
        EVENT_CLASS_APP_TYPE(SceneChanged);

        std::string ToString() const override
        {
            std::stringstream str;
            str << "SceneChangedEvent";
            return str.str();
        }
    };
}
