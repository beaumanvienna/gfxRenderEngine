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

#include <functional>

#include "marley/emulation/emulatorLayer.h"
#include "marley/emulation/bios.h"
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
void MednafenShutdown();

void SetPollEventCall(std::function<bool(SDL_Event*)> callback);
namespace Mednafen
{
    void SetLoadFailed(std::function<void()> callback);
    void SetLoad(std::function<void()> callback);
    void SetSave(std::function<void()> callback);
}

std::string gBaseDir;
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

        gBaseDir = Marley::m_EmulationUtils->GetConfigFolder();
        Marley::m_BiosFiles.QuickCheckBiosFiles();

        m_WhiteSprite = m_SpritesheetMarley->GetSprite(I_WHITE);

        ResetTargetSize();
        
        m_Load = false;
        m_Save = false;
        m_LoadFailed = false;
        m_MednafenInitialized = false;
        m_EmulatorIsRunning = false;
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
        
        if (!m_MednafenInitialized)
        {
            uint controllerCount = Input::GetControllerCount();
            for (int index = 0; index < controllerCount; index++)
            {
                gDesignatedControllers[index].joy[0]      = (pSDL_Joystick)Input::GetControllerJoy(index);
                gDesignatedControllers[index].gameCtrl[0] = (pSDL_GameController)Input::GetControllerGamecontroller(index);
            }

            SetPollEventCall([this](SDL_Event* event) { return MarleyPollEvent(event); });
            Mednafen::SetLoad([this]() { MarleyLoad(); });
            Mednafen::SetSave([this]() { MarleySave(); });
            Mednafen::SetLoadFailed([this]() { MarleyLoadFailed(); });
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

            m_MednafenInitialized = true;
            LOG_APP_INFO("mednafen initialized");
        }

        GameState::EmulationMode emulationMode = Marley::m_GameState->GetEmulationMode();
        if (emulationMode != GameState::PAUSED)
        {
            m_EmulatorIsRunning = MednafenOnUpdate();
        }
        if (m_EmulatorIsRunning)
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
                ScaleTextures();
            }

            // render
            if (m_MednafenSprite)
            {
                if (m_TargetWidth < Engine::m_Engine->GetContextWidth())
                {
                    if (!m_Instructions->IsRunning())
                    {
                        float timestep = Engine::m_Engine->GetTimestep();
                        m_TargetWidth  += 16 * timestep * 70.0f;
                        m_TargetHeight += 9 *  timestep * 70.0f;
                        m_TargetWidth  = std::min(m_TargetWidth,  Engine::m_Engine->GetContextWidth());
                        m_TargetHeight = std::min(m_TargetHeight, Engine::m_Engine->GetContextHeight());
                        ScaleTextures();
                    }
                } else
                {
                    m_Overlay->FadeOut();
                }

                // draw background
                m_SpritesheetMarley->BeginScene();
                glm::mat4 positionBG = m_WhiteSprite->GetScaleMatrix();
                glm::vec4 color(0.0f, 0.0f, 0.0f, 1.0f);
                m_Renderer->Draw(m_WhiteSprite, positionBG, 0.0f, color);

                m_Textures[0]->Bind();
                glm::vec3 translation{0.0f, 0.0f, 0.0f};

                glm::mat4 position = Translate(translation) * m_MednafenSprite->GetScaleMatrix();
                m_Renderer->Draw(m_MednafenSprite, position);

                if (m_Load)
                {
                    m_LoadTimer -= Engine::m_Engine->GetTimestep();

                    if (m_LoadTimer < 0.0f) 
                    {
                        m_Load = false;
                    }
                    
                    Sprite* sprite = m_SpritesheetMarley->GetSprite(I_DISK_LOAD);
                    glm::vec3 translation{-880.0f, -440.0f, 0.0f};
                    glm::mat4 diskPosition = Translate(translation) * sprite->GetScaleMatrix();
                    m_Renderer->Draw(sprite, diskPosition);
                }

                if (m_Save)
                {
                    m_SaveTimer -= Engine::m_Engine->GetTimestep();

                    if (m_SaveTimer < 0.0f) 
                    {
                        m_Save = false;
                    }
                    
                    Sprite* sprite = m_SpritesheetMarley->GetSprite(I_DISK_SAVE);
                    glm::vec3 translation{-880.0f, -440.0f, 0.0f};
                    glm::mat4 diskPosition = Translate(translation) * sprite->GetScaleMatrix();
                    m_Renderer->Draw(sprite, diskPosition);
                }

                if (m_LoadFailed)
                {
                    m_LoadFailedTimer -= Engine::m_Engine->GetTimestep();

                    if (m_LoadFailedTimer < 0.0f) 
                    {
                        m_LoadFailed = false;
                    }
                    
                    Sprite* sprite = m_SpritesheetMarley->GetSprite(I_DISK_EMPTY);
                    glm::vec3 translation{-880.0f, -440.0f, 0.0f};
                    glm::mat4 diskPosition = Translate(translation) * sprite->GetScaleMatrix();
                    m_Renderer->Draw(sprite, diskPosition);
                }
            }
        }
        else
        {
            QuitEmulation();
        }
    }
    
    void EmulatorLayer::QuitEmulation()
    {
        m_Overlay->FadeIn();
        ResetTargetSize();
        m_MednafenInitialized = false;
        m_EmulatorIsRunning = false;
        Marley::m_GameState->SetEmulationMode(GameState::OFF);
    }

    void EmulatorLayer::ScaleTextures()
    {
        m_Width  = mednafenWidth;
        m_Height = mednafenHeight;
        m_Textures[0]->Resize(mednafenWidth, mednafenHeight);

        float scaleX = m_TargetWidth  / mednafenWidth;
        float scaleY = m_TargetHeight / mednafenHeight;

        if (!m_MednafenSprite)
        {
            m_MednafenSprite = new Sprite(0.0f, 0.0f, 1.0f, 1.0f, mednafenWidth, mednafenHeight, m_Textures[0], "m_Textures[0]", scaleX, scaleY);
        }
        else
        {
            m_MednafenSprite->Resize(mednafenWidth, mednafenHeight);
            m_MednafenSprite->SetScale(scaleX, scaleY);
        }
        m_WhiteSprite->SetScale(m_TargetWidth, m_TargetHeight);
    }

    void EmulatorLayer::ResetTargetSize()
    {
        m_TargetWidth  = 1280.0f;
        m_TargetHeight = 720.0f;
    }

    void EmulatorLayer::OnAppEvent(AppEvent& event)
    {
        AppEventDispatcher dispatcher(event);

        dispatcher.Dispatch<EmulatorQuitEvent>([this](EmulatorQuitEvent event)
            {
                MednafenShutdown();
                QuitEmulation();
                return true;
            }
        );
    }
    
    void EmulatorLayer::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<EmulatorQuitEvent>([this](EmulatorQuitEvent event)
            {
                Marley::m_GameState->SetEmulationMode(GameState::OFF);
                return true;
            }
        );

        dispatcher.Dispatch<KeyPressedEvent>([this](KeyPressedEvent event)
            {
                SDL_KeyboardEvent keyEvent;

                switch(event.GetKeyCode())
                {
                    case ENGINE_KEY_ESCAPE:
                        if (Marley::m_GameState->GetEmulationMode() == GameState::RUNNING)
                        {
                            Marley::m_GameState->SetEmulationMode(GameState::PAUSED);
                        }
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
                    case ENGINE_KEY_F5:
                    case ENGINE_KEY_F7:
                        PushKey(keyEvent, SDL_KEYDOWN, SDL_PRESSED, false);
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
                    case ENGINE_KEY_F5:
                        keyEvent.keysym.scancode = SDL_SCANCODE_F5;
                        break;
                    case ENGINE_KEY_F7:
                        keyEvent.keysym.scancode = SDL_SCANCODE_F7;
                        break;
                }

                switch(event.GetKeyCode())
                {
                    case ENGINE_KEY_F5:
                    case ENGINE_KEY_F7:
                        PushKey(keyEvent, SDL_KEYUP, SDL_RELEASED, false);
                        break;
                }

                return false;
            }
        );
    }
    
    void EmulatorLayer::PushKey(SDL_KeyboardEvent& keyEvent, int type, int state, bool repeat)
    {
        keyEvent.type = type;
        keyEvent.timestamp = Engine::m_Engine->GetTime();
        keyEvent.state = state;
        keyEvent.repeat = repeat;
        m_SDLKeyBoardEvents.push_back(keyEvent);
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

    void EmulatorLayer::MarleyLoad()
    {
        m_LoadTimer = 1.0f;
        m_Load = true;
    }

    void EmulatorLayer::MarleySave()
    {
        m_SaveTimer = 1.0f;
        m_Save = true;
    }

    void EmulatorLayer::MarleyLoadFailed()
    {
        m_LoadFailedTimer = 1.0f;
        m_LoadFailed = true;
    }
}
