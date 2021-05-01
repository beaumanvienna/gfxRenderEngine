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

#include "engine.h"
#include "platform.h"
#include "texture.h"
#include "atlasPPSSPP.h"

class Sprite
{
    
public:

    Sprite(const uint atlasTable,
            const float pos1X, const float pos1Y, 
            const float pos2X, const float pos2Y,
            const uint width,  const uint height,
            const std::string& name);
    
    std::string GetName() const;
    
//private:

    uint m_AtlasTable;
    float m_Pos1X, m_Pos1Y, m_Pos2X, m_Pos2Y;
    uint m_Width, m_Height;
    std::string m_Name;
    
};

class SpriteAnimation
{
    
public:

    SpriteAnimation(const Sprite& sprite, uint frames);
    uint GetFrames() const { return m_Frames; }
    std::string GetPrefix() const { return m_Sprite.GetName() + "_"; }
    
private:

    Sprite m_Sprite;
    uint m_Frames;

};

typedef std::vector<Sprite> SpriteTable;           // a table of sprites
typedef std::vector<SpriteTable> SpritesheetTable; // a table of spritesheets

class SpriteSheet
{
    
public:

    SpriteSheet();
    
    bool AddSpritesheetPPSSPP(const std::string& fileName);
    bool AddSpritesheetEngine(const std::string& fileName);
    bool AddSpritesheetAnimation(const SpriteAnimation& spriteAnimation);
    Sprite* GetSprite(uint table, uint index);
    void ListSprites();
    uint GetTextureSlot() const { return m_Texture.GetTextureSlot(); }
    void BeginDrawCall() { m_Texture.Bind(); }
private:
    Texture m_Texture;
    SpritesheetTable m_SpritesheetTables;
    
};