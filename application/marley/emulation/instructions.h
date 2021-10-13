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

#include "engine.h"
#include "layer.h"
#include "buffer.h"
#include "spritesheet.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "core.h"
#include "renderer.h"
#include "transformation.h"

namespace MarleyApp
{

    class Instructions : public Layer
    {

    public:

        Instructions(std::shared_ptr<IndexBuffer> indexBuffer, std::shared_ptr<VertexBuffer> vertexBuffer, 
                std::shared_ptr<Renderer> renderer, SpriteSheet* spritesheetMarley, 
                const std::string& name = "instructions")
            : Layer(name), m_IndexBuffer(indexBuffer), m_VertexBuffer(vertexBuffer),
              m_Renderer(renderer), m_SpritesheetMarley(spritesheetMarley), m_StartTime(0)
        {
        }

        void OnAttach() override;
        void OnDetach() override;
        void OnEvent(Event& event) override;
        void OnUpdate() override;
        void Start() { if (!m_Stop) m_Start = true; }
        void Stop()  { m_Stop  = true; }
        bool IsRunning() const { return m_Running; }

    private:

        std::shared_ptr<IndexBuffer>  m_IndexBuffer;
        std::shared_ptr<VertexBuffer> m_VertexBuffer;
        std::shared_ptr<Renderer> m_Renderer;

        SpriteSheet* m_SpritesheetMarley;
        Sprite* m_InstructionsSprite;

        Animation m_InstructionsMoveIn;
        Animation m_InstructionsMoveOut;
        
        const double ON_SCREEN_TIME = 10.0f;

        bool m_Running;
        bool m_Start;
        bool m_Stop;
        
        double m_StartTime;

    };
}
