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

#include "marley/emulation/emulatorLayer.h"
#include "marley/marley.h"
#include "GL.h"
#include "core.h"
#include "glm.hpp"
#include "matrix.h"
#include "resources.h"
#include "renderCommand.h"
#include "stb_image_write.h"
#include "stb_image.h"
#include "keyEvent.h"
#include "input.h"

int mednafen_main(int argc, char* argv[]);
bool MednafenOnUpdate();

typedef bool (*pollFunctionPtr)(SDL_Event*);
void SetPollEventCall(pollFunctionPtr callback);

std::string gBaseDir = "/home/yo/.marley/";
int WINDOW_WIDTH = 1280;
int WINDOW_HEIGHT = 720;
uint mednafenWidth;
uint mednafenHeight;
int mednafenTextureIDs[4];
bool mednafenTextures;

#define MAX_DEVICES_PER_CONTROLLER 1
#define MAX_GAMEPADS 2

typedef SDL_Joystick* pSDL_Joystick;
typedef SDL_GameController* pSDL_GameController;

extern uint *src_pixies;

// controllers detected by SDL
// will be assigned a slot
typedef struct DesignatedControllers {
    pSDL_Joystick joy[MAX_DEVICES_PER_CONTROLLER];
    pSDL_GameController gameCtrl[MAX_DEVICES_PER_CONTROLLER];
    int instance[MAX_DEVICES_PER_CONTROLLER];
    int index[MAX_DEVICES_PER_CONTROLLER];
    std::string name[MAX_DEVICES_PER_CONTROLLER];
    std::string nameDB[MAX_DEVICES_PER_CONTROLLER];
    bool mappingOKDevice[MAX_DEVICES_PER_CONTROLLER];
    bool mappingOK;
    int controllerType;
    int numberOfDevices;
} T_DesignatedControllers;

T_DesignatedControllers gDesignatedControllers[MAX_GAMEPADS];

namespace MarleyApp
{
    std::vector<SDL_KeyboardEvent> EmulatorLayer::m_SDLKeyBoardEvents;
    void EmulatorLayer::OnAttach()
    {
        for(int i = 0; i < 4; i++)
        {
            mednafenTextureIDs[i] = 0;
            m_Textures[i].reset();
        }

        mednafenTextures = false;
        m_MednafenSprite = nullptr;
        m_Width = m_Height = mednafenWidth = mednafenHeight = 0;
    }

    void EmulatorLayer::OnDetach()
    {
    }

    void EmulatorLayer::BeginScene()
    {
    }

    void EmulatorLayer::EndScene()
    {
    }

    void EmulatorLayer::OnUpdate()
    {
        static bool mednafenInitialized = false;
        if (!mednafenInitialized)
        {
            uint controllerCount = Input::GetControllerCount();
            for (int index = 0; index < controllerCount; index++)
            {
                gDesignatedControllers[index].joy[0]      = (pSDL_Joystick)Input::GetControllerJoy(index);
                gDesignatedControllers[index].gameCtrl[0] = (pSDL_GameController)Input::GetControllerGamecontroller(index);
            }

            SetPollEventCall(MarleyPollEvent);
            m_SDLKeyBoardEvents.clear();

            int argc = 2;
            char *argv[10];
            char arg1[1024];
            char arg2[1024];

            std::string str = "mednafen";
            strcpy(arg1, str.c_str());

            strcpy(arg2, m_GameFilename.c_str());

            argv[0] = arg1;
            argv[1] = arg2;

            mednafen_main(argc, argv);

            mednafenInitialized = true;
            LOG_APP_INFO("mednafen initialized");
        }

        if (MednafenOnUpdate())
        {
            if (mednafenTextures && !m_Textures[0])
            {
                mednafenTextures = false;
                {
                    for (int i =0; i < 4; i++)
                    {
                        m_Textures[i] = Texture::Create(mednafenTextureIDs[i], GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
                    }
                }
            }

            if ((m_Width != mednafenWidth) || (m_Height != mednafenHeight))
            {
                m_Width  = mednafenWidth;
                m_Height = mednafenHeight;
                m_Textures[0]->Resize(mednafenWidth, mednafenHeight);
                if (!m_MednafenSprite)
                {
                    m_MednafenSprite = new Sprite(0.0f, 0.0f, 1.0f, 1.0f, mednafenWidth, mednafenHeight, m_Textures[0], "m_Textures[0]", 5.0f, 3.2142f);
                }
                else
                {
                    m_MednafenSprite->Resize(mednafenWidth, mednafenHeight);
                }
            }

            // render
            if (m_MednafenSprite)
            {
                m_Textures[0]->Bind();
                glm::vec3 translation{0.0f, 0.0f, 0.0f};

                glm::mat4 position = Translate(translation) * m_MednafenSprite->GetScaleMatrix();
                m_Renderer->Draw(m_MednafenSprite, position);
            }
        }
        else
        {
            mednafenInitialized = false;
            Marley::m_GameState->SetEmulationMode(GameState::OFF);
        }
    }

    void EmulatorLayer::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<KeyPressedEvent>([this](KeyPressedEvent event)
            {
                SDL_KeyboardEvent keyEvent;

                switch(event.GetKeyCode())
                {
                    case ENGINE_KEY_ESCAPE:
                        keyEvent.keysym.scancode = SDL_SCANCODE_ESCAPE;
                        break;
                    case ENGINE_KEY_F5:
                        keyEvent.keysym.scancode = SDL_SCANCODE_F5;
                        break;
                    case ENGINE_KEY_F7:
                        keyEvent.keysym.scancode = SDL_SCANCODE_F7;
                        break;
                }

                switch(event.GetKeyCode())
                {
                    case ENGINE_KEY_ESCAPE:
                    case ENGINE_KEY_F5:
                    case ENGINE_KEY_F7:
                        keyEvent.type = SDL_KEYDOWN;
                        keyEvent.timestamp = Engine::m_Engine->GetTime();
                        keyEvent.state = SDL_PRESSED;
                        keyEvent.repeat = false;
                        m_SDLKeyBoardEvents.push_back(keyEvent);
                        break;
                }
                return false;
            }
        );

        dispatcher.Dispatch<KeyReleasedEvent>([this](KeyReleasedEvent event)
            {
                SDL_KeyboardEvent keyEvent;

                switch(event.GetKeyCode())
                {
                    case ENGINE_KEY_ESCAPE:
                        keyEvent.keysym.scancode = SDL_SCANCODE_ESCAPE;
                        break;
                    case ENGINE_KEY_F5:
                        keyEvent.keysym.scancode = SDL_SCANCODE_F5;
                        break;
                    case ENGINE_KEY_F7:
                        keyEvent.keysym.scancode = SDL_SCANCODE_F7;
                        break;
                }

                switch(event.GetKeyCode())
                {
                    case ENGINE_KEY_ESCAPE:
                    case ENGINE_KEY_F5:
                    case ENGINE_KEY_F7:
                        keyEvent.type = SDL_KEYUP;
                        keyEvent.timestamp = Engine::m_Engine->GetTime();
                        keyEvent.state = SDL_RELEASED;
                        keyEvent.repeat = false;
                        m_SDLKeyBoardEvents.push_back(keyEvent);
                        break;
                }

                return false;
            }
        );
    }

    bool EmulatorLayer::MarleyPollEvent(SDL_Event* event)
    {
        bool eventAvailable = m_SDLKeyBoardEvents.size();
        if (eventAvailable)
        {
            event[0].key = m_SDLKeyBoardEvents.back();
            m_SDLKeyBoardEvents.pop_back();
        }
        return eventAvailable;
    }
}
