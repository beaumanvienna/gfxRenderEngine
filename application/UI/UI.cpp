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

#include "UI.h"
#include "mainScreen.h"
#include "controllerEvent.h"
#include "mouseEvent.h"
#include "keyEvent.h"
#include "inputState.h"
#include "texture.h"

Sprite* whiteImage;
std::unique_ptr<SCREEN_ScreenManager> UI::m_ScreenManager = nullptr;
std::shared_ptr<Texture> UI::m_FontAtlas;

void UI::OnAttach()
{
    m_ScreenManager = std::make_unique<SCREEN_ScreenManager>(m_Renderer, m_SpritesheetMarley);
    
    m_FontAtlas = Texture::Create();
    m_FontAtlas->Init("resources/atlas/fontAtlas.png");
    m_FontAtlas->Bind();
    
    MainScreen* mainScreen = new MainScreen(m_SpritesheetMarley);
    mainScreen->OnAttach();
    m_ScreenManager->push(mainScreen);
    
    whiteImage = m_SpritesheetMarley->GetSprite(I_WHITE);
}

void UI::OnDetach() 
{
}

void UI::OnUpdate()
{
    m_ScreenManager->update();
    m_ScreenManager->render();
}

void UI::OnEvent(Event& event)
{
    if (!m_ScreenManager) 
    {
        return;
    }
    
    EventDispatcher dispatcher(event);

    dispatcher.Dispatch<ControllerButtonPressedEvent>([this](ControllerButtonPressedEvent event) 
        { 
            SCREEN_KeyInput key;
            key.flags = KEY_DOWN;
            key.keyCode = event.GetControllerButton();
            key.deviceId = DEVICE_ID_PAD_0;
            m_ScreenManager->key(key);
            
            return true;
        }
    );

    dispatcher.Dispatch<ControllerButtonReleasedEvent>([this](ControllerButtonReleasedEvent event) 
        { 
            SCREEN_KeyInput key;
            key.flags = KEY_UP;
            key.keyCode = event.GetControllerButton();
            key.deviceId = DEVICE_ID_PAD_0;
            m_ScreenManager->key(key);
            
            return true;
        }
    );
    
    dispatcher.Dispatch<MouseButtonPressedEvent>([this](MouseButtonPressedEvent event) 
        { 
            if (event.GetButton() == MouseButtonEvent::Left) 
            {
            }
            return true;
        }
    );

    dispatcher.Dispatch<KeyPressedEvent>([this](KeyPressedEvent event) 
        { 
            switch(event.GetKeyCode())
            {
                case ENGINE_KEY_ENTER:
                    
                    break;
            }
            return false;
        }
    );
    
}


