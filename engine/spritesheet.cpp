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

#include "spritesheet.h"
#include "log.h"
#include "OpenGL/GL.h"
#include "ui_atlas.h"
#include "../../resources/images/ui_atlas/ui_atlas.cpp"

Sprite::Sprite(const uint atlasTable,
       const float pos1X, const float pos1Y, 
       const float pos2X, const float pos2Y,
       const uint  width, const uint  height,
       const std::string& name) :
            m_AtlasTable(atlasTable),
            m_Pos1X(pos1X), m_Pos1Y(pos1Y), 
            m_Pos2X(pos2X), m_Pos2Y(pos2Y),
            m_Width(width), m_Height(height),
            m_Name(name)
{
}

std::string Sprite::GetName() const
{
    return m_Name;
}

SpriteAnimation::SpriteAnimation(uint frames, uint millisecondsPerFrame, SpriteSheet* spritesheet) :
    m_Frames(frames), m_MillisecondsPerFrame(millisecondsPerFrame), m_Spritesheet(spritesheet)
{
    m_Duration = static_cast<double>(m_Frames * m_MillisecondsPerFrame / 1000);
}

void SpriteAnimation::Create(uint frames, uint millisecondsPerFrame, SpriteSheet* spritesheet)
{
    m_Spritesheet = spritesheet;
    m_Frames  = frames;
    m_MillisecondsPerFrame = millisecondsPerFrame;
    m_Duration = static_cast<double>(m_Frames * m_MillisecondsPerFrame / 1000);
}

Sprite* SpriteAnimation::GetSprite()
{
    Sprite* sprite;
    if (IsRunning())
    {
        uint index = static_cast<int>((glfwGetTime() - m_StartTime) * 1000 / ( m_MillisecondsPerFrame ));
        sprite = m_Spritesheet->GetSprite(0,index);
    }
    else
    {
        sprite = m_Spritesheet->GetSprite(0,0);
    }
    return sprite;
}

void SpriteAnimation::Start()
{ 
    m_StartTime = glfwGetTime(); 
}

bool SpriteAnimation::IsRunning()
{ 
    return (glfwGetTime() - m_StartTime) < m_Duration; 
}

SpriteSheet::SpriteSheet()
{
}

bool SpriteSheet::AddSpritesheetPPSSPP(const std::string& fileName)
{
    bool ok = true;
    m_Texture.Create(fileName);
    SpriteTable spriteTable;
    int spritesheetTableCurrentIndex = m_SpritesheetTables.size();
    for (int i = 0; i < ui_atlas.num_images; i++)
    {
        Sprite sprite = Sprite
        (
            spritesheetTableCurrentIndex,
            ui_images[i].u1,
            1 - ui_images[i].v1,
            ui_images[i].u2,
            1 - ui_images[i].v2,
            ui_images[i].w,
            ui_images[i].h,
            ui_images[i].name
        );
        spriteTable.push_back(sprite);
    }
    m_SpritesheetTables.push_back(spriteTable);
    return ok;
}

bool SpriteSheet::AddSpritesheetEngine(const std::string& fileName)
{
    bool ok = true;
    Texture texture(fileName);
    return ok;

}

void SpriteSheet::ListSprites()
{
    int i = 0;
    int j = 0;
    for(auto table : m_SpritesheetTables)
    {
        i = 0;
        for(auto sprite : table)
        {
            std::string infoMessage = 
                "Found sprite, name: " + sprite.GetName() +
                ", table: " + std::to_string(j) +
                ", index: " + std::to_string(i);
            Log::GetLogger()->info(infoMessage);
            i++;
        }
        j++;
    }
}

Sprite* SpriteSheet::GetSprite(uint table, uint index)
{
    return &m_SpritesheetTables[table][index];
}

bool SpriteSheet::AddSpritesheetAnimation(const std::string& fileName, uint frames, uint millisecondsPerFrame)
{
    bool ok = true;
    m_Texture.Create(fileName);
    m_SpriteAnimation.Create(frames, millisecondsPerFrame, this);
    SpriteTable spriteTable;
    int spritesheetTableCurrentIndex = m_SpritesheetTables.size();
    std::string prefix = "_";
    
    float sprite_normalized_width = 1.0f / frames;
    float sprite_width = m_Texture.GetWidth() / frames;
    float sprite_height = m_Texture.GetHeight();
    
    for (int i = 0; i < frames; i++)
    {
        std::string name = prefix + std::to_string(i);
        
        Sprite sprite = Sprite
        (
            spritesheetTableCurrentIndex,
            i * sprite_normalized_width,       //u1
            1.0f,                              //v1
            (i + 1) * sprite_normalized_width, //u1
            0.0f,                              //v2
            sprite_width,                       //w
            sprite_height,                      //h
            name
        );
        spriteTable.push_back(sprite);
    }
    m_SpritesheetTables.push_back(spriteTable);
    return ok;
}
