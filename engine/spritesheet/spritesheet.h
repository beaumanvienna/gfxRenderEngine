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
#include "glm.hpp"
#include "atlas.h"

struct AtlasImage {
    float u1, v1, u2, v2;
    int w, h;
    int rotation;
    char name[32];
};

struct Atlas 
{
    const AtlasImage *images = nullptr;
    const int num_images = 0;
};

class Sprite
{
    
public:
    Sprite(const uint atlasTable,
            const float pos1X, const float pos1Y, 
            const float pos2X, const float pos2Y,
            const uint width,  const uint height,
            const std::shared_ptr<Texture> texture,
            const std::string& name,
            const float scale = 1.0f);
    
    Sprite(const uint atlasTable,
            const float pos1X, const float pos1Y, 
            const float pos2X, const float pos2Y,
            const uint width,  const uint height,
            const std::shared_ptr<Texture> texture,
            const std::string& name,
            const float scale,
            const bool rotated);

    Sprite(const uint atlasTable,
            const float pos1X, const float pos1Y, 
            const float pos2X, const float pos2Y,
            const uint width,  const uint height,
            const std::shared_ptr<Texture> texture,
            const std::string& name,
            const float scaleX,
            const float scaleY);
    
    std::string GetName() const;
    
    const glm::mat4& GetScaleMatrix(bool flipped = false);
    uint GetTextureSlot() const { return m_Texture->GetTextureSlot(); }
    void SetScaleMatrix(const float scale);
    void SetScaleMatrix(const float scaleX, const float scaleY);
    float GetWidth() const { return static_cast<float>(m_Width) * m_ScaleX; }
    float GetHeight() const { return static_cast<float>(m_Height) * m_ScaleY; }

    uint m_AtlasTable;
    float m_Pos1X, m_Pos1Y, m_Pos2X, m_Pos2Y;
    uint m_Width, m_Height;
    bool m_Rotated;
    std::shared_ptr<Texture> m_Texture;
    
private:
    void SetScaleMatrix();
    
private:
    std::string m_Name;
    float m_ScaleX;
    float m_ScaleY;
    glm::mat4 m_ScaleMatrix;
    glm::mat4 m_FlippedScaleMatrix;
};

class SpriteSheet;
class SpriteAnimation
{
    
public:
    
    SpriteAnimation() {}
    SpriteAnimation(uint frames, uint millisecondsPerFrame, SpriteSheet* spritesheet);
    void Create(uint frames, uint millisecondsPerFrame, SpriteSheet* spritesheet);
    uint GetFrames() const { return m_Frames; }
    uint GetCurrentFrame() const;
    bool IsNewFrame();
    void Start();
    bool IsRunning();
    Sprite* GetSprite();
private:
    SpriteSheet* m_Spritesheet;
    uint m_Frames;
    uint m_MillisecondsPerFrame;
    double m_StartTime = 0;
    double m_Duration;
    float m_TimeFactor;
    uint m_PreviousFrame;
};

typedef std::vector<Sprite> SpriteTable;           // a table of sprites
typedef std::vector<SpriteTable> SpritesheetTable; // a table of spritesheets

class SpriteSheet
{
    
public:

    SpriteSheet();
    
    bool AddSpritesheetPPSSPP(const std::string& fileName);
    bool AddSpritesheetEngine(const std::string& fileName);
    bool AddSpritesheetAnimation(const std::string& fileName, uint frames, uint millisecondsPerFrame, const float scale = 1.0f);
    bool AddSpritesheetAnimation(const std::string& fileName, uint frames, uint millisecondsPerFrame, const float scaleX, const float scaleY);
    bool AddSpritesheetAnimation(Sprite* originalSprite, uint frames, uint millisecondsPerFrame, const float scale = 1.0f);
    bool AddSpritesheetRow(Sprite* originalSprite, uint frames, const float scale = 1.0f);
    Sprite* GetSprite(uint table, uint index);
    Sprite* GetSprite(uint index);
    SpriteAnimation* GetSpriteAnimation() { return &m_SpriteAnimation; }
    void ListSprites();
    uint GetTextureSlot() const { return m_Texture->GetTextureSlot(); }
    void BeginScene() { m_Texture->Bind(); }
    
private:

    std::shared_ptr<Texture> m_Texture;
    SpriteAnimation m_SpriteAnimation;
    SpritesheetTable m_SpritesheetTables;
    
};
