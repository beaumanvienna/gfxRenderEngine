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
#include "sprite.h"
#include "texture.h"
#include "glm.hpp"
#include "atlas.h"

struct AtlasImage
{
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

typedef std::vector<Sprite> SpriteTable;

class SpriteSheet
{
    
public:

    SpriteSheet();
    
    bool AddSpritesheet(const std::string& fileName);
    bool AddSpritesheet(const char* path /* Linux */, int resourceID /* Windows */, const std::string& resourceClass /* Windows */);
    bool AddSpritesheetTile(const std::string& fileName, const std::string& mapName, uint columns, uint rows, uint spacing, const float scale = 1.0f);
    bool AddSpritesheetTile(const char* path /* Linux */, int resourceID /* Windows */, const std::string& resourceClass /* Windows */, 
                            const std::string& mapName, uint columns, uint rows, uint spacing, const float scale = 1.0f);
    bool AddSpritesheetRow(Sprite* originalSprite, uint frames, const float scaleX, const float scaleY);
    bool AddSpritesheetRow(Sprite* originalSprite, uint frames, const float scale = 1.0f);
    bool AddSpritesheetRow(const std::string& fileName, uint frames, const float scaleX, const float scaleY);
    bool AddSpritesheetRow(const std::string& fileName, uint frames, const float scale = 1.0f);
    bool AddSpritesheetRow(const char* path /* Linux */, int resourceID /* Windows */, const std::string& resourceClass /* Windows */, 
                           uint frames, const float scaleX, const float scaleY);
    bool AddSpritesheetRow(const char* path /* Linux */, int resourceID /* Windows */, const std::string& resourceClass /* Windows */, 
                           uint frames, const float scale = 1.0f);
    Sprite* GetSprite(uint index);
    void ListSprites();
    uint GetTextureSlot() const { return m_Texture->GetTextureSlot(); }
    std::shared_ptr<Texture> GetTexture() const { return m_Texture; }
    uint GetNumberOfSprites() const { return m_SpriteTable.size(); }
    uint GetRows() const { return m_Rows; }
    uint GetColumns() const { return m_Columns; }
    void BeginScene() { m_Texture->Bind(); }

private:

    void AddSpritesheet();
    void AddSpritesheetTile(const std::string& mapName, uint columns, uint rows, uint spacing, const float scale);

private:

    std::shared_ptr<Texture> m_Texture;
    SpriteTable m_SpriteTable;
    uint m_Rows, m_Columns;
    
};
