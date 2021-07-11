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

#include "mapIndex.h"

MapIndex::MapIndex()
    : m_Index(0), m_Rows(0), m_Columns(0), 
      m_Spritesheet(nullptr), 
      m_MaxIndicies(0)
{
}

Sprite* MapIndex::GetSprite()
{
    Sprite* sprite = nullptr;
    if (m_Index < m_MaxIndicies)
    {
        if (m_Map[m_Index] != EMPTY)
        {
            sprite = m_Spritesheet->GetSprite(m_Map[m_Index]);
        }
        m_Index++;
    }
    else
    {
        LOG_CORE_ERROR("Sprite* MapIndex::GetSprite(): index overflow");
    }
    
    return sprite;
}

bool MapIndex::Create(SpriteSheet* spritesheet)
{
    m_Spritesheet = spritesheet;

    return (spritesheet != nullptr);
}

void MapIndex::AddRectangularTileGroup(const char* id, const glm::vec2& start, uint width, uint height)
{
    std::shared_ptr<TileGroup> tileGroup = std::make_shared<TileGroup>(start, width, height);
    m_TileGroupMap.insert(std::make_pair(id[0], tileGroup));
}

void MapIndex::AddMap(const char* map)
{
    m_Rows = 0;
    m_Columns = 0;
    m_MaxIndicies = 0;
    if (!m_Spritesheet)
    {
        LOG_CORE_ERROR("void MapIndex::AddMap(const char* map): sprite sheet not initialized");
        return;
    }

    uint length = strlen(map);
    bool insideRow = false;
    m_Map.clear();
    for (uint i = 0; i < length; i++)
    {
        if (map[i] == '|')
        {
            if (!insideRow)
            {
                insideRow = true;
            }
            else
            {
                insideRow = false;
                m_Rows++;
                AdvanceTileGroupsY();
            }
        }
        else
        {
            if(!m_Rows)
            {
                m_Columns++;
            }
            if (map[i] - ASCII_SPACE)
            {
                m_Map.push_back(GetSpritesheetIndex(map[i]));
            }
            else
            {
                m_Map.push_back(EMPTY);
            }
        }
    }
    m_MaxIndicies = m_Rows * m_Columns;
}

int MapIndex::GetSpritesheetIndex(const char id)
{
    std::shared_ptr<TileGroup> tileGroup = m_TileGroupMap[id];
    
    if (tileGroup)
    {
        int index = tileGroup->m_CurrentPosition.x + m_Spritesheet->GetColumns()*tileGroup->m_CurrentPosition.y;
        tileGroup->AdvancePositionX();
        return index;
    }
    return EMPTY;
}

void MapIndex::AdvanceTileGroupsY()
{
    for (auto& [key, tileGroup] : m_TileGroupMap)
    {
        tileGroup->AdvancePositionY();
    }
}
