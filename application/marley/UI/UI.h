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

#include "engine.h"
#include "layer.h"
#include "buffer.h"
#include "renderer.h"
#include "spritesheet.h"
#include "screen.h"
#include "marley/UI/stars.h"
#include "marley/UI/mainScreen.h"
#include "marley/UI/settingsTabs/controllerSetupAnimation.h"

namespace MarleyApp
{

    class UI : public Layer
    {

    public:

        UI(std::shared_ptr<IndexBuffer> indexBuffer, std::shared_ptr<VertexBuffer> vertexBuffer, 
                std::shared_ptr<Renderer> renderer, SpriteSheet* spritesheetMarley, 
                const std::string& name = "layer")
            : Layer(name), m_IndexBuffer(indexBuffer), m_VertexBuffer(vertexBuffer),
              m_Renderer(renderer), m_SpritesheetMarley(spritesheetMarley)
        {
        }

        void OnAttach() override;
        void OnDetach() override;
        void OnEvent(Event& event) override;
        void OnUpdate() override;

        static std::unique_ptr<SCREEN_ScreenManager> m_ScreenManager;
        static std::shared_ptr<Texture> m_FontAtlas;
        static std::shared_ptr<Texture> m_ImageAtlas;

    private:

        bool Touch(int flags, float x, float y, int deviceID);
        void Key(int keyFlag, int keyCode, int deviceID);
        void Axis();

    private:

        std::shared_ptr<IndexBuffer>  m_IndexBuffer;
        std::shared_ptr<VertexBuffer> m_VertexBuffer;
        std::shared_ptr<Renderer> m_Renderer;

        UIStarIcon*               m_UIStarIcon            = nullptr;
        MainScreen*               m_MainScreen            = nullptr;
        ControllerSetupAnimation* m_UIControllerAnimation = nullptr;

        // sprite sheets
        SpriteSheet* m_SpritesheetMarley;

    };
}
