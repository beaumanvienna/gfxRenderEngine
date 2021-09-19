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
#include "GL.h"
#include "core.h"
#include "glm.hpp"
#include "matrix.h"
#include "resources.h"
#include "renderCommand.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include "input.h"

int mednafen_main(int argc, char* argv[]);
void MednafenOnUpdate();

std::string gBaseDir = "/home/yo/.marley/";
int WINDOW_WIDTH = 1280;
int WINDOW_HEIGHT = 720;
uint gMainBuffer[256*224];

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

        FramebufferTextureSpecification textureSpec(FramebufferTextureFormat::RGBA8);
        FramebufferAttachmentSpecification fbAttachments{textureSpec};

        // frame buffer
        m_FbSpec = FramebufferSpecification {256, 224, fbAttachments, 1, false};
        m_Framebuffer = Framebuffer::Create(m_FbSpec);

        // framebuffer texture
        m_FramebufferTexture = Texture::Create();
        m_FramebufferTexture->Init(m_FbSpec.m_Width, m_FbSpec.m_Height, m_Framebuffer->GetColorAttachmentRendererID(0));

        // framebuffer sprite
        m_FramebufferSprite = new Sprite(0.0f, 0.0f, 1.0f, 1.0f, m_FramebufferTexture->GetWidth(), m_FramebufferTexture->GetHeight(), m_FramebufferTexture, "framebuffer texture", 5.0f, 3.2142f);

        size_t fileSize;
        const uchar* buffer = (const uchar*)ResourceSystem::GetDataPointer(fileSize, "/images/images/I_DK.png", IDB_DK, "PNG");

        m_Pixels = stbi_load_from_memory(buffer, fileSize, &m_Width, &m_Height, &m_BPP, 4);
        uint x = 0;
        uint y = 0;
        m_FramebufferTexture->Blit(x, y, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, m_Pixels);

        for (int i = 0; i < 256 * 224; i++) gMainBuffer[i] = 0xff000000;
    }

    void EmulatorLayer::OnDetach() 
    {
        if (m_FramebufferSprite) delete m_FramebufferSprite;
    }

    void EmulatorLayer::BeginScene()
    {
        m_Framebuffer->Bind();
    }

    void EmulatorLayer::EndScene()
    {
        m_Framebuffer->Unbind();
        GLCall(glViewport(0, 0, Engine::m_Engine->GetWindowWidth(), Engine::m_Engine->GetWindowHeight()));
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

            int argc    = 2;
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
        
        MednafenOnUpdate();
        uint x = 0;
        uint y = 0;

        m_FramebufferTexture->Blit(x, y, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, gMainBuffer);

        

        // render frame buffer
        {
            m_FramebufferTexture->Bind();
            glm::vec3 translation{0.0f, 0.0f, 0.0f};

            glm::mat4 position = Translate(translation) * m_FramebufferSprite->GetScaleMatrix();
            m_Renderer->Draw(m_FramebufferSprite, position);
        }
    }

    void EmulatorLayer::OnEvent(Event& event)  {}
}