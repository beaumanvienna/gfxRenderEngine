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

#include "layer.h"
#include "engine.h"
#include "buffer.h"
#include "sprite.h"
#include "texture.h"
#include "renderer.h"
#include "scabb/rayTracing/aux.h"
#include "scabb/rayTracing/global.h"
#include "gtc/matrix_transform.hpp"
#include "glm.hpp"

using point3 = glm::vec3;
using color  = glm::vec3;

namespace ScabbApp
{
    class RayTracing : public Layer
    {

    public:

        RayTracing(std::shared_ptr<IndexBuffer> indexBuffer, std::shared_ptr<VertexBuffer> vertexBuffer, 
                        std::shared_ptr<Renderer> renderer, SpriteSheet* spritesheetMarley,
                        const std::string& name = "RayTracing")
            : Layer(name), m_IndexBuffer(indexBuffer), m_VertexBuffer(vertexBuffer),
              m_Renderer(renderer), m_SpritesheetMarley(spritesheetMarley),
              m_Progress(0.0f)
        {
        }

        void OnAttach() override;
        void OnDetach() override;
        void OnEvent(Event& event) override;
        void OnUpdate() override;

    private:

        void ReportProgress(float progress);

    private:
        std::shared_ptr<IndexBuffer>  m_IndexBuffer;
        std::shared_ptr<VertexBuffer> m_VertexBuffer;
        std::shared_ptr<Renderer> m_Renderer;
        SpriteSheet* m_SpritesheetMarley;

        std::shared_ptr<Texture> m_CanvasTexture;
        Sprite* m_ProgressIndicator;
        Sprite* m_Canvas;

        float m_Progress;

    };
}
