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

#include <cmath>

#include "UI.h"
#include "texture.h"
#include "keyEvent.h"
#include "mouseEvent.h"
#include "inputState.h"
#include "marley/marley.h"
#include "controllerEvent.h"
#include "instrumentation.h"
#include "marley/UI/settingsScreen.h"
#include "resources.h"
#include "input.h"

namespace MarleyApp
{

    Sprite* whiteImage;
    std::unique_ptr<SCREEN_ScreenManager> UI::m_ScreenManager = nullptr;
    std::shared_ptr<Texture> UI::m_FontAtlas;
    std::shared_ptr<Texture> UI::m_ImageAtlas;

    void UI::OnAttach()
    {
        m_ScreenManager = std::make_unique<SCREEN_ScreenManager>(m_Renderer, m_SpritesheetMarley);

        m_FontAtlas = ResourceSystem::GetTextureFromMemory("/images/atlas/fontAtlas.png", IDB_FONTS_RETRO, "PNG");

        m_ImageAtlas = m_SpritesheetMarley->GetTexture();

        m_MainScreen = new MainScreen(m_SpritesheetMarley);
        m_MainScreen->OnAttach();
        m_ScreenManager->push(m_MainScreen);

        whiteImage = m_SpritesheetMarley->GetSprite(I_WHITE);

        m_UIStarIcon = new UIStarIcon(m_IndexBuffer, m_VertexBuffer, m_Renderer, m_SpritesheetMarley, false, "UI star icon");
        Engine::m_Engine->PushOverlay(m_UIStarIcon);

        m_UIControllerAnimation = new ControllerSetupAnimation(m_IndexBuffer, m_VertexBuffer, m_Renderer, m_SpritesheetMarley, "controller animation");
        Engine::m_Engine->PushOverlay(m_UIControllerAnimation);
    }

    void UI::OnDetach() 
    {
        m_MainScreen->OnDetach();
    }

    void UI::OnUpdate()
    {
        PROFILE_FUNCTION();
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

        if ( (SettingsScreen::m_IsCintrollerSetupScreen) && Input::ConfigurationRunning())
        {
            m_UIControllerAnimation->SetActiveController(Input::GetConfigurationActiveController());
            m_UIControllerAnimation->SetFrame(Input::GetConfigurationStep());
            m_UIControllerAnimation->OnUpdate();
        }

        m_UIStarIcon->OnUpdate();

        Axis();

    }

    void UI::OnEvent(Event& event)
    {
        if ( (!m_ScreenManager) || Marley::m_GameState->EmulationIsRunning())
        {
            return;
        }

        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<ControllerButtonPressedEvent>([this](ControllerButtonPressedEvent event) 
            {
                Key(KEY_DOWN, event.GetControllerButton(), DEVICE_ID_PAD_0);
                return true;
            }
        );

        dispatcher.Dispatch<ControllerButtonReleasedEvent>([this](ControllerButtonReleasedEvent event) 
            {
                Key(KEY_UP, event.GetControllerButton(), DEVICE_ID_PAD_0);
                return true;
            }
        );

        dispatcher.Dispatch<MouseButtonPressedEvent>([this](MouseButtonPressedEvent event) 
            {
                if (event.GetButton() == MouseButtonEvent::Left) 
                {
                    // output context coordinates adjusted for orthographic projection
                    float windowScale = Engine::m_Engine->GetWindowScale();
                    float contextPositionX = event.GetX()/windowScale;
                    float contextPositionY = event.GetY()/windowScale;

                    int flags = TOUCH_DOWN | TOUCH_MOUSE;
                    float x = contextPositionX;
                    float y = contextPositionY;
                    int deviceID = 0;
                    return Touch(flags, x, y, deviceID);
                }
                return false;
            }
        );

        dispatcher.Dispatch<MouseButtonReleasedEvent>([this](MouseButtonReleasedEvent event) 
            {
                if (event.GetMouseButton() == MouseButtonEvent::Left) 
                {
                    int flags = TOUCH_UP | TOUCH_MOUSE;
                    float x = 0.0f;
                    float y = 0.0f;
                    int deviceID = 0;
                    return Touch(flags, x, y, deviceID);
                }
                return false;
            }
        );

        dispatcher.Dispatch<MouseScrolledEvent>([this](MouseScrolledEvent event) 
            {
                int flags = TOUCH_WHEEL;
                float x = 0.0f;
                float y = event.GetY();
                int deviceID = 0;
                return Touch(flags, x, y, deviceID);
            }
        );

        dispatcher.Dispatch<KeyPressedEvent>([this](KeyPressedEvent event) 
            { 
                Key(KEY_DOWN, event.GetKeyCode(), DEVICE_ID_KEYBOARD);
                return false;
            }
        );

        dispatcher.Dispatch<KeyReleasedEvent>([this](KeyReleasedEvent event) 
            { 
                Key(KEY_UP, event.GetKeyCode(), DEVICE_ID_KEYBOARD);
                return false;
            }
        );
    }

    bool UI::Touch(int flags, float x, float y, int deviceID)
    {
        if (Marley::m_GameState->GetScene() != GameState::SPLASH)
        {
            SCREEN_TouchInput touch;
            touch.x = x;
            touch.y = y;
            touch.flags = flags;
            touch.id = deviceID;
            touch.timestamp = Engine::m_Engine->GetTime();
            return m_ScreenManager->touch(touch);
        }
        return false;
    }

    void UI::Key(int keyFlag, int keyCode, int deviceID)
    {
        if (Marley::m_GameState->GetScene() != GameState::SPLASH)
        {
            SCREEN_KeyInput key;
            key.flags = keyFlag;
            key.keyCode = keyCode;
            key.deviceId = deviceID;
            m_ScreenManager->key(key);
        }
    }

    void UI::Axis()
    {
        if (!Input::GetControllerCount()) return;
        glm::vec2 controllerAxisInput = Input::GetControllerStick(Controller::FIRST_CONTROLLER, Controller::RIGHT_STICK);

        SCREEN_AxisInput axis;
        axis.flags = 0;
        axis.deviceId = DEVICE_ID_PAD_0;
        if (std::abs(controllerAxisInput.x) > std::abs(controllerAxisInput.y))
        {
            axis.axisId = Controller::RIGHT_STICK_HORIZONTAL;
            axis.value  = controllerAxisInput.x;
        }
        else
        {
            axis.axisId = Controller::RIGHT_STICK_VERTICAL;
            axis.value  = controllerAxisInput.y;
        }
        m_ScreenManager->axis(axis);
    }
}
