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

#include "core.h"
#include "layer.h"
#include "engine.h"
#include "buffer.h"
#include "spritesheet.h"
#include "spriteAnimation.h"
#include "marley/characters/InputHandler.h"
#include "gtc/matrix_transform.hpp"
#include "orthographicCamera.h"
#include "controllerEvent.h"
#include "renderer.h"
#include "tetragon.h"
#include "glm.hpp"
#include "event.h"

namespace MarleyApp
{

    constexpr float LIMIT_LEFT  = -192.0f;
    constexpr float LIMIT_RIGHT =  900.4f;
    constexpr float LIMIT_UP    =  -60.0f;
    constexpr float LIMIT_DOWN  = -440.0f;

    class Overlay : public Layer
    {

    public:

        Overlay(std::shared_ptr<IndexBuffer> indexBuffer, std::shared_ptr<VertexBuffer> vertexBuffer,
                std::shared_ptr<Renderer> renderer, SpriteSheet* spritesheetMarley,
                const std::string& name = "layer")
            : Layer(name), m_IndexBuffer(indexBuffer), m_VertexBuffer(vertexBuffer),
              m_Renderer(renderer), m_SpritesheetMarley(spritesheetMarley),
              m_Rotation(0), m_FrameTranslationX(0.0f),
              m_TranslationSpeed(200.0f), m_Translation(nullptr),
              m_WhiteSprite(nullptr), m_Alpha(1.0f),
              m_FadeIn(false), m_FadeOut(false)
        {
        }

        void OnAttach() override;
        void OnDetach() override;
        void OnEvent(Event& event) override;
        void OnUpdate() override;
        void SetRotation(float rotation) { m_Rotation = rotation; }

        void FadeIn()  { m_FadeIn = true;  m_FadeOut = false; }
        void FadeOut() { m_FadeIn = false; m_FadeOut = true;  }

        // event processing
        void OnControllerButtonPressed(ControllerButtonPressedEvent& event);
        void OnControllerButtonReleased(ControllerButtonReleasedEvent& event);

    private:

        void Fade();

        std::shared_ptr<IndexBuffer>  m_IndexBuffer;
        std::shared_ptr<VertexBuffer> m_VertexBuffer;
        std::shared_ptr<Renderer>     m_Renderer;
        std::unique_ptr<InputHandler> m_InputHandler;

        Sprite* m_WhiteSprite;

        // sprite sheets
        SpriteSheet* m_SpritesheetMarley;
        SpriteSheet m_SpritesheetHorn;
        SpriteAnimation m_HornAnimation;
        SpriteSheet m_SpritesheetWalk;
        SpriteAnimation m_WalkAnimation;
        SpriteSheet m_SpritesheetWalkUp;
        SpriteAnimation m_WalkUpAnimation;
        SpriteSheet m_SpritesheetWalkDown;
        SpriteAnimation m_WalkDownAnimation;

        glm::vec3* m_Translation;
        float m_TranslationSpeed;

        float m_Rotation;

        float m_FrameTranslationX;
        float m_GuybrushWalkDelta;
        float m_GuybrushWalkUpDelta;
        float m_GuybrushWalkDownDelta;

        bool m_FadeIn;
        bool m_FadeOut;
        float m_Alpha;

    };
}
