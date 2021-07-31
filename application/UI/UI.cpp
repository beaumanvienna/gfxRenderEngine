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
#include "settingsScreen.h"
#include "resources.h"

Sprite* whiteImage;
std::unique_ptr<SCREEN_ScreenManager> UI::m_ScreenManager = nullptr;
std::shared_ptr<Texture> UI::m_FontAtlas;
std::shared_ptr<Texture> UI::m_ImageAtlas;

void UI::OnAttach()
{
    m_ScreenManager = std::make_unique<SCREEN_ScreenManager>(m_Renderer, m_SpritesheetMarley);
    
    m_FontAtlas = ResourceSystem::GetTextureFromMemory("/images/atlas/fontAtlas.png", IDB_FONTS_RETRO, "PNG");
    
    m_ImageAtlas = m_SpritesheetMarley->GetTexture();
    
    MainScreen* mainScreen = new MainScreen(m_SpritesheetMarley);
    mainScreen->OnAttach();
    m_ScreenManager->push(mainScreen);
    
    whiteImage = m_SpritesheetMarley->GetSprite(I_WHITE);
    
    m_UIStarIcon = new UIStarIcon(m_IndexBuffer, m_VertexBuffer, m_Renderer, m_SpritesheetMarley, "UI star icon");
    Engine::m_Engine->PushOverlay(m_UIStarIcon);
}

void UI::OnDetach() 
{
}

void UI::OnUpdate()
{
    m_ScreenManager->update();
    m_ScreenManager->render();
    
    if (SettingsScreen::m_IsCreditsScreen)
    {
        m_UIStarIcon->Start();
    }
    else
    {
        m_UIStarIcon->Stop();
    }
    m_UIStarIcon->OnUpdate();
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
            SCREEN_KeyInput key;
            key.flags = KEY_DOWN;
            key.keyCode = event.GetKeyCode();
            key.deviceId = DEVICE_ID_KEYBOARD;
            m_ScreenManager->key(key);
            
            return false;
        }
    );

    dispatcher.Dispatch<KeyReleasedEvent>([this](KeyReleasedEvent event) 
        { 
            SCREEN_KeyInput key;
            key.flags = KEY_UP;
            key.keyCode = event.GetKeyCode();
            key.deviceId = DEVICE_ID_KEYBOARD;
            m_ScreenManager->key(key);
            
            return false;
        }
    );
    
}
