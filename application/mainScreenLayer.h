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
#include "event.h"
#include "core.h"
#include "orthographicCamera.h"
#include "transformation.h"
#include "renderer.h"

class MainScreenLayer : public Layer
{
    
public:

    MainScreenLayer(std::shared_ptr<IndexBuffer> indexBuffer, std::shared_ptr<VertexBuffer> vertexBuffer, 
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
    
private:

    void InitAnimation();
    
private:
    std::shared_ptr<IndexBuffer>  m_IndexBuffer;
    std::shared_ptr<VertexBuffer> m_VertexBuffer;
    std::shared_ptr<Renderer> m_Renderer;

    // sprite sheets
    SpriteSheet* m_SpritesheetMarley;

    Animation cloudAnimationRight, cloudAnimationLeft, tabAnimation;
    Sprite* m_CloudSprite;
    Sprite* m_BeachSprite;
    Sprite* m_TabSprite;

};
