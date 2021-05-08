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

#include "engine.h"
#include "platform.h"
#include <functional>

enum class EventType
{
    None = 0,
    WindowClose,
    KeyPressed,
    KeyReleased,
    MouseButtonPressed,
    MouseButtonReleased,
    MouseMoved,
    MouseScrolled,
    ControllerButtonPressed,
    ControllerButtonReleased,
    ControllerAxisMoved
};

enum EventCategory
{
    None = 0,
    EventCategoryApplication      = BIT(0),
    EventCategoryInput            = BIT(1),
    EventCategoryKeyboard         = BIT(2),
    EventCategoryMouse            = BIT(3),
    EventCategoryMouseButton      = BIT(4),
    EventCategoryController       = BIT(5),
    EventCategoryControllerButton = BIT(6)
};

#define EVENT_CLASS_CATEGORY(x) int GetCategoryFlags() const override { return x; }
#define EVENT_CLASS_TYPE(x) static EventType GetStaticType() { return EventType::x; }\
            EventType GetEventType() const override { return GetStaticType(); }\
            const char* GetName() const override { return  #x "Event"; }

class Event
{
    
    friend class EventDispatcher;
    
public:

    virtual EventType GetEventType() const = 0;
    virtual const char* GetName() const = 0;
    virtual int GetCategoryFlags() const = 0;
    virtual std::string ToString() const { return GetName(); }
    
    inline bool IsInCategory(EventCategory category)
    {
        return GetCategoryFlags() & category;
    }
    
protected:

    bool m_Handled = false;

};
                
class EventDispatcher
{
    template <typename T>
    using EventFn = std::function<bool(T&)>;
    
public:
    EventDispatcher(Event& event)
        : m_Event(event)
    {
    }
    
    template<typename T>
    bool Dispatch(EventFn<T> func)
    {
        if (m_Event.GetEventType() == T::GetStaticType())
        {
            m_Event.m_Handled = func(*(T*)&m_Event);
            return true;
        }
        return false;
    }
    
private:

    Event& m_Event;
    
};
