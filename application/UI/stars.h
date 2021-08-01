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

class UIStarIcon : public Layer
{
    
public:

    UIStarIcon(std::shared_ptr<IndexBuffer> indexBuffer, std::shared_ptr<VertexBuffer> vertexBuffer, 
            std::shared_ptr<Renderer> renderer, SpriteSheet* spritesheetMarley, bool narrow,
            const std::string& name = "layer")
        : Layer(name), m_IndexBuffer(indexBuffer), m_VertexBuffer(vertexBuffer),
          m_Renderer(renderer), m_SpritesheetMarley(spritesheetMarley), m_Narrow(narrow)
    {
    }
    
    void OnAttach() override;
    void OnDetach() override;
    void OnEvent(Event& event) override;
    void OnUpdate() override;
    void Start() { m_Start = true; }
    void Stop()  { m_Stop  = true; }
    bool IsRunning() const { return m_Running; }
    
private:

    std::shared_ptr<IndexBuffer>  m_IndexBuffer;
    std::shared_ptr<VertexBuffer> m_VertexBuffer;
    std::shared_ptr<Renderer> m_Renderer;

    SpriteSheet* m_SpritesheetMarley;
    Sprite* m_StarSprite;

    Animation m_StarMoveIn1;
    Animation m_StarMoveIn2;
    Animation m_StarMoveIn3;
    Animation m_StarRotate1;
    Animation m_StarRotate2;
    Animation m_StarRotate3;
    Animation m_StarMoveOut1;
    Animation m_StarMoveOut2;
    Animation m_StarMoveOut3;
    bool m_Running;
    bool m_Start;
    bool m_Stop;
    bool m_Narrow;

};
