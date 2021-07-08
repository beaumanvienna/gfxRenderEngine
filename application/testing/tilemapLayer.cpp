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

#include "tilemapLayer.h"
#include "matrix.h"
#include <gtx/transform.hpp>

extern float duration;
constexpr uint tileColumns = 27;
constexpr uint tileRows = 18;
void TilemapLayer::OnAttach() 
{
    m_TileMap.AddSpritesheetTile("application/appResources/urban/tilemap/tilemap.png", "urban", tileColumns, tileRows, 1);
}

void TilemapLayer::OnDetach() 
{
}

void TilemapLayer::OnUpdate()
{
    m_TileMap.BeginScene();
    uint spriteIndex = 0;
    
    //for (uint column = 0; column < tileColumns; column++)
    for (uint row = 0; row < tileRows; row++)
    {
        for (uint column = 0; column < tileColumns; column++)
        {
            Sprite* sprite = m_TileMap.GetSprite(spriteIndex);
            float translationX = static_cast<float>(column * sprite->GetWidth()) - 458.0f;
            float translationY = 305.0f - static_cast<float>(row * sprite->GetHeight());
            
            sprite->SetScaleMatrix(2.0f);
            glm::vec3 translation{translationX, translationY, 0.0f};
            glm::mat4 translationMatrix = Translate(translation);
            // transformed position
            glm::mat4 position = translationMatrix * sprite->GetScaleMatrix();
            
            m_Renderer->Draw(sprite, position);
            spriteIndex++;
        }
    }
}

void TilemapLayer::OnEvent(Event& event) 
{
}


