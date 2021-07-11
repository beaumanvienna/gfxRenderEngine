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

#include <vector>
#include <map>

#include "engine.h"
#include "spritesheet.h"
#include "glm.hpp"

class MapIndex
{

public:

    MapIndex();

    Sprite* GetSprite();
    bool Create(SpriteSheet* spritesheet);
    void BeginScene() { m_Index = 0; }
    void AddRectangularTileGroup(const char* id, const glm::vec2& start, uint width, uint height);
    void AddMap(const char* map);
    uint GetRows() const { return m_Rows; }
    uint GetColumns() const { return m_Columns; }
    
private:
    
    struct TileGroup
    {
        TileGroup(glm::vec2 start, uint width, uint height) 
            : m_Start(start), m_CurrentPosition(start),
              m_Width(width), m_Height(height)
        {}
        
        void AdvancePositionX()
        {
            if ((m_CurrentPosition.x + 1) < (m_Start.x + m_Width))
            {
                m_CurrentPosition.x++;
            }
            else
            {
                m_CurrentPosition.x = m_Start.x;
            }
        }
        void AdvancePositionY() 
        {
            if ((m_CurrentPosition.y + 1) < (m_Start.y + m_Height))
            {
                m_CurrentPosition.x = m_Start.x;
                m_CurrentPosition.y++;
            }
            else
            {
                m_CurrentPosition = m_Start;
            }
        }
        
        glm::vec2 m_Start, m_CurrentPosition;
        uint m_Width;
        uint m_Height;
    };
    
private:

    int GetSpritesheetIndex(const char id);
    void AdvanceTileGroupsY();

private:

    static constexpr uint ASCII_SPACE = ' ';
    static constexpr int EMPTY = -1;

    SpriteSheet* m_Spritesheet;
    uint m_Index, m_MaxIndicies;
    uint m_Rows, m_Columns;
    
    std::vector<int> m_Map;
    std::map<const char, std::shared_ptr<TileGroup>> m_TileGroupMap;

};
