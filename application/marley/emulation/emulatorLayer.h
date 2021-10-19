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

#include <memory>
#include <vector>

#include "engine.h"
#include "layer.h"
#include "buffer.h"
#include "renderer.h"
#include "spritesheet.h"
#include "marley/emulation/instructions.h"
#include "marley/characters/GuybrushWalk.h"
#include "marley/appEvent.h"
#include "framebuffer.h"
#include "SDL.h"

namespace MarleyApp
{

    class EmulatorLayer : public Layer
    {

    public:

        EmulatorLayer(std::shared_ptr<IndexBuffer> indexBuffer, std::shared_ptr<VertexBuffer> vertexBuffer,
                std::shared_ptr<Renderer> renderer, SpriteSheet* spritesheetMarley, Overlay* overlay,
                const std::string& name = "EmulatorLayer")
            : Layer(name), m_IndexBuffer(indexBuffer), m_VertexBuffer(vertexBuffer), m_Renderer(renderer),
              m_SpritesheetMarley(spritesheetMarley), m_WhiteSprite(nullptr), m_GameFilename(""),
              m_Overlay(overlay), m_TargetWidth(0.0f), m_TargetHeight(0.0f) {}

        void OnAttach() override;
        void OnDetach() override;
        void OnEvent(Event& event) override;
        void OnUpdate() override;
        void BeginScene();
        void EndScene();
        void OnAppEvent(AppEvent& event);
        void QuitEmulation();

        void PushKey(SDL_KeyboardEvent& keyEvent, int type, int state, bool repeat);
        void SetGameFilename(const std::string& gameFilename) { m_GameFilename = gameFilename; }
        void SetInstructions(Instructions* instructions) { m_Instructions = instructions; }
        bool MarleyPollEvent(SDL_Event* event);
        void MarleyLoadFailed();
        void MarleyLoad();
        void MarleySave();
        void ResetTargetSize();
        void ScaleTextures();

    private:

        std::shared_ptr<IndexBuffer>  m_IndexBuffer;
        std::shared_ptr<VertexBuffer> m_VertexBuffer;
        std::shared_ptr<Renderer> m_Renderer;

        // sprite sheets
        SpriteSheet* m_SpritesheetMarley;
        Sprite* m_MednafenSprite;
        Sprite* m_WhiteSprite;

        std::shared_ptr<Texture> m_Textures[4];
        int m_Width, m_Height;

        std::string m_GameFilename;
        Instructions* m_Instructions;
        Overlay* m_Overlay;
        float m_TargetWidth;
        float m_TargetHeight;
        bool m_MednafenInitialized;
        bool m_EmulatorIsRunning;
        std::vector<SDL_KeyboardEvent> m_SDLKeyBoardEvents;

        float m_LoadFailedTimer;
        bool m_LoadFailed;

        float m_LoadTimer;
        bool m_Load;

        float m_SaveTimer;
        bool m_Save;

    };
}
