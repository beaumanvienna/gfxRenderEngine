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
#include "embeddedResources.h"

extern bool showTileMap;

void TilemapLayer::OnAttach() 
{
    m_TileMap.AddSpritesheetTile("application/appResources/urban/tilemap/tilemap.png", "urban", TILE_COLUMNS, TILE_ROWS, 1, 1.5f);
    
    m_MapIndex.Create(&m_TileMap);
    m_MapIndex.AddRectangularTileGroup("A", {1,15}, 1 /* width */, 3 /* height */);
    m_MapIndex.AddRectangularTileGroup("B", {0,15}, 1 /* width */, 3 /* height */);
    
    m_MapIndex.AddMap
    (
        "|     |"
        "|     |"
        "|     |"
        "|AABAA|"
        "|AABAA|"
        "|AABAA|"
        "|     |"
        "|     |"
    );

#ifdef LINUX
    size_t file_size = 0;
    GBytes *mem_access = g_resource_lookup_data(embeddedResources_get_resource(), "/images/atlas/atlas.png", G_RESOURCE_LOOKUP_FLAGS_NONE, nullptr);
    const void* dataPtr = g_bytes_get_data(mem_access, &file_size);

    if (dataPtr != nullptr && file_size)
    {
        LOG_CORE_CRITICAL("    pointer to memory buffer is valid");

        m_AtlasTexture = Texture::Create();
        m_AtlasTexture->Init("resources/atlas/atlas.png");
        //m_AtlasTexture->Init(2048, 2048, dataPtr);
        m_AtlasTexture->Bind();
        
        m_Atlas = new Sprite
        (
            0.0f, 1.0f,
            1.0f, 0.0f,
            m_AtlasTexture->GetWidth(), m_AtlasTexture->GetHeight(),
            m_AtlasTexture,
            "resources/atlas/atlas.png", 0.3f
        );
    }
    else
    {
        m_Atlas = nullptr;
    }
#endif
}

void TilemapLayer::OnDetach() 
{
}

void TilemapLayer::OnUpdate()
{

    if (showTileMap)
    {
#ifdef LINUX
        if (m_Atlas)
        {
            float translationX = -100.0f;
            float translationY = 200.0f;

            glm::vec3 translation{translationX, translationY, 0.0f};
            glm::mat4 translationMatrix = Translate(translation);
            // transformed position
            glm::mat4 position = translationMatrix * m_Atlas->GetScaleMatrix();

            m_Renderer->Draw(m_Atlas, position);
        }
#endif
        {
            m_MapIndex.BeginScene();
            Sprite* sprite;
        
            for (uint row = 0; row < m_MapIndex.GetRows(); row++)
            {
                for (uint column = 0; column < m_MapIndex.GetColumns(); column++)
                {
                    sprite = m_MapIndex.GetSprite();
                    if (sprite)
                    {
                        float translationX = static_cast<float>(column * sprite->GetWidth()) + 650.0f;
                        float translationY = 150.0f - static_cast<float>(row * sprite->GetHeight());
                        
                        glm::vec3 translation{translationX, translationY, 0.0f};
                        glm::mat4 translationMatrix = Translate(translation);
                        // transformed position
                        glm::mat4 position = translationMatrix * sprite->GetScaleMatrix();
                        
                        m_Renderer->Draw(sprite, position);
                    }
                }
            }
        }
        
        {
            m_TileMap.BeginScene();
            uint spriteIndex = 0;
        
            for (uint row = 0; row < TILE_ROWS; row++)
            {
                for (uint column = 0; column < TILE_COLUMNS; column++)
                {
                    Sprite* sprite = m_TileMap.GetSprite(spriteIndex);
                    float translationX = static_cast<float>(column * sprite->GetWidth()) - 500.0f;
                    float translationY = -100.0f - static_cast<float>(row * sprite->GetHeight());
                    
                    glm::vec3 translation{translationX, translationY, 0.0f};
                    glm::mat4 translationMatrix = Translate(translation);
                    // transformed position
                    glm::mat4 position = translationMatrix * sprite->GetScaleMatrix();
                    
                    m_Renderer->Draw(sprite, position);
                    spriteIndex++;
                }
            }
        }
    }
}

void TilemapLayer::OnEvent(Event& event) 
{
}


