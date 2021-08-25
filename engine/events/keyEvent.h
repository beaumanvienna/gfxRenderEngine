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
#include "event.h"

class KeyEvent : public Event
{

public:
    
    inline int GetKeyCode() const { return m_KeyCode; }
    
    EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard);

protected:
    KeyEvent(int keyCode)
        : m_KeyCode(keyCode)
    {
    }

private:

    int m_KeyCode;

};

class KeyPressedEvent : public KeyEvent
{

public:
    KeyPressedEvent(int keyCode)
        : KeyEvent(keyCode)
    {
    }

    EVENT_CLASS_TYPE(KeyPressed);
    
    std::string ToString() const override
    {
        std::stringstream str;
        str << "KeyPressedEvent: m_KeyCode: " << GetKeyCode();
        return str.str();
    }

};

class KeyReleasedEvent : public KeyEvent
{

public:
    KeyReleasedEvent(int keyCode)
        : KeyEvent(keyCode)
    {
    }
    
    EVENT_CLASS_TYPE(KeyReleased);
    
    std::string ToString() const override
    {
        std::stringstream str;
        str << "KeyReleasedEvent: m_KeyCode: " << GetKeyCode();
        return str.str();
    }

};
