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
#include "event.h"

class ControllerAxisMovedEvent : public Event
{

public:

    ControllerAxisMovedEvent(int axis, float x, float y)
        : m_Axis(axis), m_ControllerX(x), m_ControllerY(y)
    {
    }
    
    inline float GetX() const { return m_ControllerX; }
    inline float GetY() const { return m_ControllerY; }

    EVENT_CLASS_CATEGORY(EventCategoryController);
    EVENT_CLASS_TYPE(ControllerAxisMoved);

private:
    
    int m_Axis;
    float m_ControllerX, m_ControllerY;

};

class ControllerButtonEvent : public Event
{

public:
    
    inline int GetControllerButton()  { return m_ControllerButton; }
    
    EVENT_CLASS_CATEGORY(EventCategoryController | EventCategoryControllerButton);
    
protected:
    ControllerButtonEvent(int controllerButton)
        : m_ControllerButton(controllerButton)
    {
    }
    
private:

    int m_ControllerButton;

};

class ControllerButtonPressedEvent : public ControllerButtonEvent
{

public:
    ControllerButtonPressedEvent(int controllerButton)
        : ControllerButtonEvent(controllerButton)
    {
    }

    EVENT_CLASS_TYPE(ControllerButtonPressed);

};

class ControllerButtonReleasedEvent : public ControllerButtonEvent
{

public:
    ControllerButtonReleasedEvent(int controllerButton)
        : ControllerButtonEvent(controllerButton)
    {
    }
    
    EVENT_CLASS_TYPE(ControllerButtonReleased);

};
