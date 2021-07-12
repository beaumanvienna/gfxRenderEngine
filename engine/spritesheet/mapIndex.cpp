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

// sprite sheet: a tile map with rows, columns, and spacing; created with SpriteSheet::AddSpritesheetTile
// m_TileGroupMap: maps letters to tile groups in a sprite sheet (= rectangular sub area of the tile sprite sheet)
// map from void MapIndex::AddMap(const char* map): original ASCII map with '|' delimeters, read-only
// m_CharMap: copy of map, read/write, letters will be removed when the indices for a group are found
// m_IndexMap: same as m_CharMap, only that letters are resolved into indices of tiles in the sprite sheet

MapIndex::MapIndex()
    : m_Index(0), m_Rows(0), m_Columns(0), 
      m_Spritesheet(nullptr), 
      m_CharMapSize(0)
{
}

Sprite* MapIndex::GetSprite()
{
    Sprite* sprite = nullptr;
    if (m_Index < m_CharMapSize)
    {
        if (m_IndexMap[m_Index] != EMPTY)
        {
            sprite = m_Spritesheet->GetSprite(m_IndexMap[m_Index]);
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
    if (!m_Spritesheet)
    {
        LOG_CORE_ERROR("void MapIndex::AddMap(const char* map): sprite sheet not initialized");
        return;
    }

    // set up m_Rows, m_Columns, m_CharMapSize
    m_Rows = 0;
    m_Columns = 0;
    m_IndexMap.clear();
    bool insideRow = false;
    uint length = strlen(map);
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
            }
        }
        else
        {
            m_CharMap.push_back(map[i]);
            m_IndexMap.push_back(EMPTY);
            
            if(!m_Rows)
            {
                m_Columns++;
            }
        }
    }
    m_CharMapSize = m_Rows * m_Columns;
    
    // create index map
    for (uint i = 0; i < m_CharMapSize; i++)
    {
        if (m_CharMap[i] - ASCII_SPACE)
        {
            SetIndexMap(i, m_CharMap[i]);
        }
    }
}

void MapIndex::SetIndexMap(const uint charMapStartIndex, const char id)
{
    std::shared_ptr<TileGroup> tileGroup = m_TileGroupMap[id];

    if (tileGroup)
    {
        uint offsetX = 0;
        uint offsetY = 0;
        bool lastElementInGroup = false;
        do
        {
            int spritesheetIndex = tileGroup->m_CurrentPosition.x + m_Spritesheet->GetColumns()*tileGroup->m_CurrentPosition.y;
            uint charMapIndex    = charMapStartIndex + offsetX + offsetY * m_Columns;
            m_IndexMap[charMapIndex] = spritesheetIndex;
            
            // mark in char map as processed
            m_CharMap[charMapIndex]  = ASCII_SPACE;
            
            // advance in tile group to next tile
            lastElementInGroup = tileGroup->AdvancePosition(&offsetX, &offsetY);
        } while (!lastElementInGroup);
    }
}

void MapIndex::PrintCharMap() const
{
    for (uint row = 0; row < m_Rows; row++)
    {
        std::cout << '|';
        for (uint column = 0; column < m_Columns; column++)
        {
            uint charMapIndex = column + row * m_Columns;
            std::cout << m_CharMap[charMapIndex];
        }
        std::cout << '|' << std::endl;
    }
}

void MapIndex::PrintIndexMap() const
{
    for (uint row = 0; row < m_Rows; row++)
    {
        std::cout << '|';
        for (uint column = 0; column < m_Columns; column++)
        {
            uint charMapIndex = column + row * m_Columns;
            std::cout << m_IndexMap[charMapIndex] << ",";
        }
        std::cout << '|' << std::endl;
    }
}
