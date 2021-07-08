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

#include "core.h"
#include "spritesheet.h"
#include "log.h"
#include "atlas.h"
#include "../../resources/atlas/atlas.cpp"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include <gtx/transform.hpp>
#include "matrix.h"

Sprite::Sprite(const uint atlasTable,
       const float pos1X, const float pos1Y, 
       const float pos2X, const float pos2Y,
       const uint  width, const uint  height,
       const std::shared_ptr<Texture> texture,
       const std::string& name,
       const float scale) :
            m_AtlasTable(atlasTable),
            m_Pos1X(pos1X), m_Pos1Y(pos1Y), 
            m_Pos2X(pos2X), m_Pos2Y(pos2Y),
            m_Width(width), m_Height(height),
            m_Texture(texture), m_ScaleX(scale),
            m_Name(name), m_ScaleY(scale), 
            m_Rotated(false)
{
    SetScaleMatrix();
}

Sprite::Sprite(const uint atlasTable,
       const float pos1X, const float pos1Y, 
       const float pos2X, const float pos2Y,
       const uint  width, const uint  height,
       const std::shared_ptr<Texture> texture,
       const std::string& name,
       const float scale,
       const bool rotated) :
            m_AtlasTable(atlasTable),
            m_Pos1X(pos1X), m_Pos1Y(pos1Y), 
            m_Pos2X(pos2X), m_Pos2Y(pos2Y),
            m_Width(width), m_Height(height),
            m_Texture(texture), m_ScaleX(scale),
            m_Name(name), m_ScaleY(scale), 
            m_Rotated(rotated)
{
    SetScaleMatrix();
}

Sprite::Sprite(const uint atlasTable,
       const float pos1X, const float pos1Y, 
       const float pos2X, const float pos2Y,
       const uint  width, const uint  height,
       const std::shared_ptr<Texture> texture,
       const std::string& name,
       const float scaleX, const float scaleY) :
            m_AtlasTable(atlasTable),
            m_Pos1X(pos1X), m_Pos1Y(pos1Y), 
            m_Pos2X(pos2X), m_Pos2Y(pos2Y),
            m_Width(width), m_Height(height),
            m_Texture(texture), m_Name(name),
            m_ScaleX(scaleX), m_ScaleY(scaleY),
            m_Rotated(false)
{
    SetScaleMatrix();
}

std::string Sprite::GetName() const
{
    return m_Name;
}

void Sprite::SetScaleMatrix(const float scale)
{
    m_ScaleX = m_ScaleY = scale;
    SetScaleMatrix();
}

void Sprite::SetScaleMatrix(const float scaleX, const float scaleY)
{
    m_ScaleX = scaleX;
    m_ScaleY = scaleY;
    SetScaleMatrix();
}

void Sprite::SetScaleMatrix()
{
    float spriteWidth = static_cast<float>(m_Width);
    float spriteHeight = static_cast<float>(m_Height);
        
    glm::mat4 spriteMatrix = glm::mat4
    (
        -spriteWidth,  spriteHeight, 1.0f, 1.0f,
        spriteWidth,   spriteHeight, 1.0f, 1.0f,
        spriteWidth,  -spriteHeight, 1.0f, 1.0f,
        -spriteWidth, -spriteHeight, 1.0f, 1.0f
    );
    
    // model matrix
    glm::vec3 scaleVec(m_ScaleX/2.0f, m_ScaleY/2.0f, 1.0f);
    if (m_Rotated)
    {
        m_ScaleMatrix = Rotate(Matrix::NINETY_DEGREES, {0.0f,0.0f,1.0f}) * Scale(scaleVec) * spriteMatrix;
    }
    else
    {
        m_ScaleMatrix = Scale(scaleVec) * spriteMatrix;
    }
}

const glm::mat4& Sprite::GetScaleMatrix(bool flipped)
{ 
    if (!flipped) return m_ScaleMatrix; 
    
    float x0 = m_ScaleMatrix[0][0];
    float x1 = m_ScaleMatrix[1][0];
    float x2 = m_ScaleMatrix[2][0];
    float x3 = m_ScaleMatrix[3][0];
    
    m_FlippedScaleMatrix = m_ScaleMatrix;
    
    if (!m_Rotated)
    {
        m_FlippedScaleMatrix[0][0] = x1;
        m_FlippedScaleMatrix[1][0] = x0;
        m_FlippedScaleMatrix[2][0] = x3;
        m_FlippedScaleMatrix[3][0] = x2;
        
        return m_FlippedScaleMatrix;
    }
    else
    {
        m_FlippedScaleMatrix[0][0] = x3;
        m_FlippedScaleMatrix[1][0] = x2;
        m_FlippedScaleMatrix[2][0] = x1;
        m_FlippedScaleMatrix[3][0] = x1;
        
        return m_FlippedScaleMatrix;
    }
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
    m_Duration = static_cast<double>(m_Frames * m_MillisecondsPerFrame / 1000.0f);
    m_TimeFactor = 1000.0f / static_cast<float>(m_MillisecondsPerFrame);
}

Sprite* SpriteAnimation::GetSprite()
{
    Sprite* sprite;
    if (IsRunning())
    {
        uint index = static_cast<int>((Engine::m_Engine->GetTime() - m_StartTime) * m_TimeFactor);
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
    m_PreviousFrame = -1;
    m_StartTime = Engine::m_Engine->GetTime();
}

bool SpriteAnimation::IsRunning()
{ 
    return (Engine::m_Engine->GetTime() - m_StartTime) < m_Duration; 
}

uint SpriteAnimation::GetCurrentFrame() const 
{ 
    return static_cast<uint>((Engine::m_Engine->GetTime() - m_StartTime) * m_TimeFactor); 
}

bool SpriteAnimation::IsNewFrame()
{ 
    uint currentFrame = GetCurrentFrame();
    bool isNewFrame = (currentFrame != m_PreviousFrame);
    m_PreviousFrame = currentFrame;
    return isNewFrame;
}

SpriteSheet::SpriteSheet()
{
    m_Texture = Texture::Create();
}

bool SpriteSheet::AddSpritesheetPPSSPP(const std::string& fileName)
{
    bool ok = true;
    m_Texture->Init(fileName);
    SpriteTable spriteTable;
    int spritesheetTableCurrentIndex = m_SpritesheetTables.size();
    for (int i = 0; i < atlas.num_images; i++)
    {
        bool rotated = images[i].rotation;
        Sprite sprite = Sprite
        (
            spritesheetTableCurrentIndex,
            images[i].u1,
            images[i].v1,
            images[i].u2,
            images[i].v2,
            images[i].w,
            images[i].h,
            m_Texture,
            images[i].name,
            1.0f,
            rotated
        );
        spriteTable.push_back(sprite);
    }
    m_SpritesheetTables.push_back(spriteTable);
    return ok;
}

bool SpriteSheet::AddSpritesheetTile(const std::string& fileName, const std::string& mapName, uint columns, uint rows, uint spacing)
{
    bool ok = m_Texture->Init(fileName);
    if (ok)
    {
        SpriteTable spriteTable;
        
        int tileWidth             = (m_Texture->GetWidth()  - spacing * (columns - 1))/columns;
        int tileHeight            = (m_Texture->GetHeight() - spacing * (rows - 1))/rows;
        
        float tileWidthNormalized  = static_cast<float>(tileWidth)  / m_Texture->GetWidth();
        float tileHeightNormalized = static_cast<float>(tileHeight) / m_Texture->GetHeight();
        
        float advanceX            = static_cast<float>(tileWidth  + spacing)  / m_Texture->GetWidth();
        float advanceY            = static_cast<float>(tileHeight + spacing) / m_Texture->GetHeight();
        
        
        int spritesheetTableCurrentIndex = m_SpritesheetTables.size();
        
        float currentY = 0.0f;
        for (uint row = 0; row < rows; row++)
        {
            float currentX = 0.0f;
            for (uint column = 0; column < columns; column++)
            {
                std::string name = mapName + "_" + std::to_string(row) + "_" + std::to_string(column);
                bool rotated = false;
                float u1 = currentX;
                float v1 = 1.0f - currentY;
                float u2 = currentX + tileWidthNormalized;
                float v2 = 1.0f - (currentY + tileHeightNormalized);
                Sprite sprite = Sprite
                (
                    spritesheetTableCurrentIndex,
                    u1,
                    v1,
                    u2,
                    v2,
                    tileWidth,
                    tileHeight,
                    m_Texture,
                    name,
                    1.0f,
                    rotated
                );
                spriteTable.push_back(sprite);
                currentX += advanceX;
            }
            currentY += advanceY;
        }
        m_SpritesheetTables.push_back(spriteTable);
    }
    else
    {
        LOG_CORE_CRITICAL("Couldn't load {0}", fileName);
    }
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
            LOG_CORE_INFO("Found sprite, name: {0}, table: {1}, index: {2}", sprite.GetName(), std::to_string(j), std::to_string(i));
            i++;
        }
        j++;
    }
}

Sprite* SpriteSheet::GetSprite(uint table, uint index)
{
    return &m_SpritesheetTables[table][index];
}

Sprite* SpriteSheet::GetSprite(uint index)
{
    return GetSprite(0, index);
}

bool SpriteSheet::AddSpritesheetAnimation(const std::string& fileName, uint frames, uint millisecondsPerFrame, const float scale)
{
    return AddSpritesheetAnimation(fileName, frames, millisecondsPerFrame, scale, scale);
}

bool SpriteSheet::AddSpritesheetAnimation(const std::string& fileName, uint frames, uint millisecondsPerFrame, const float scaleX, const float scaleY)
{
    bool ok = true;
    m_Texture->Init(fileName);
    m_SpriteAnimation.Create(frames, millisecondsPerFrame, this);
    SpriteTable spriteTable;
    int spritesheetTableCurrentIndex = m_SpritesheetTables.size();
    std::string prefix = "_";
    
    float sprite_normalized_width = 1.0f / frames;
    float sprite_width = m_Texture->GetWidth() / frames;
    float sprite_height = m_Texture->GetHeight();
    
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
            m_Texture,
            name,
            scaleX,
            scaleY
        );
        spriteTable.push_back(sprite);
    }
    m_SpritesheetTables.push_back(spriteTable);
    return ok;
}

bool SpriteSheet::AddSpritesheetAnimation(Sprite* originalSprite, uint frames, uint millisecondsPerFrame, const float scale)
{
    bool ok = true;
    SpriteTable spriteTable;
    
    m_SpriteAnimation.Create(frames, millisecondsPerFrame, this);
    std::string prefix = "_";
    m_Texture = originalSprite->m_Texture;
    
    if (!originalSprite->m_Rotated)
    {
        float sprite_normalized_width = (originalSprite->m_Pos2X - originalSprite->m_Pos1X) / frames;
        float sprite_width = originalSprite->m_Width / frames;
        float sprite_height = originalSprite->m_Height;
        
        for (int i = 0; i < frames; i++)
        {
            std::string name = prefix + std::to_string(i);
            
            Sprite sprite = Sprite
            (
                originalSprite->m_AtlasTable,
                originalSprite->m_Pos1X + i * sprite_normalized_width,       //u1
                originalSprite->m_Pos1Y,                                     //v1
                originalSprite->m_Pos1X + (i + 1) * sprite_normalized_width, //u1
                originalSprite->m_Pos2Y,                                     //v2
                sprite_width,                                                //w
                sprite_height,                                               //h
                originalSprite->m_Texture,
                name,
                scale
            );
            spriteTable.push_back(sprite);
        }
    }
    else
    {
        float sprite_normalized_height = (originalSprite->m_Pos2Y - originalSprite->m_Pos1Y) / frames;
        float sprite_width = originalSprite->m_Width;
        float sprite_height = originalSprite->m_Height / frames;
        
        for (int i = 0; i < frames; i++)
        {
            std::string name = prefix + std::to_string(i);
            
            Sprite sprite = Sprite
            (
                originalSprite->m_AtlasTable,
                originalSprite->m_Pos1X,                                      //u1
                originalSprite->m_Pos1Y  + i * sprite_normalized_height,      //v1
                originalSprite->m_Pos2X,                                      //u1
                originalSprite->m_Pos1Y + (i + 1) * sprite_normalized_height, //v2
                sprite_width,                                                 //w
                sprite_height,                                                //h
                originalSprite->m_Texture,
                name,
                scale,
                true
            );
            spriteTable.push_back(sprite);
        }
    }
    m_SpritesheetTables.push_back(spriteTable);
    return ok;
}

bool SpriteSheet::AddSpritesheetRow(Sprite* originalSprite, uint frames, const float scale)
{
    return AddSpritesheetAnimation(originalSprite, frames, 0, scale);
}
