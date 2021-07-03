/* Copyright (c) 2013-2020 PPSSPP project
   https://github.com/hrydgard/ppsspp/blob/master/LICENSE.TXT
   
   Engine Copyright (c) 2021 Engine Development Team 
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

#include <iostream>
#include <vector>
#include <stdio.h>
#include <string.h>

#include "spritesheet.h"

#define ATLAS_MAGIC ('A' + ('T' << 8) + ('L' << 16) | ('A' << 24))

// Metadata file structure v0:
//
// AtlasHeader
// For each image:
//   AtlasImage
// For each font:
//   AtlasFontHeader
//   For each range:
//     AtlasRange
//   For each char:
//     AtlasChar

struct SCREEN_Atlas;

struct SCREEN_ImageID {
public:
    SCREEN_ImageID() : id(nullptr) {}
    ~SCREEN_ImageID() {}
    explicit SCREEN_ImageID(const char *_id) : id(_id) {}
    explicit SCREEN_ImageID(const char *_id, int currentFrame) : id(_id), currentFrame_(currentFrame)
    {
        isSpriteSheet = true;
    }

    static inline SCREEN_ImageID invalid() {
        return SCREEN_ImageID{ nullptr };
    }

    bool isValid() const {
        return id != nullptr;
    }

    bool isInvalid() const {
        return id == nullptr;
    }

    bool operator ==(const SCREEN_ImageID &other) {
        return (id == other.id) || !strcmp(id, other.id);
    }

    bool operator !=(const SCREEN_ImageID &other) {
        if (id == other.id) {
            return false;
        }
        return strcmp(id, other.id) != 0;
    }

    bool isSpriteSheet = false;
    int currentFrame_ = 0; // requested frame
    
private:
    const char *id;

    friend struct SCREEN_Atlas;
};

struct FontID 
{
public:
    explicit FontID(const char *_id) 
        : id(_id) {}

    static inline FontID invalid() { return FontID{ nullptr }; }

    bool isInvalid() const { return id == nullptr; }

private:
    const char *id;
    friend struct SCREEN_Atlas;
};

struct AtlasChar 
{
    // texcoords
    float sx, sy, ex, ey;
    // offset from the origin
    float ox, oy;
    // distance to move the origin forward
    float wx;
    // size in pixels
    unsigned short pw, ph;
};

struct AtlasCharRange 
{
    int start;
    int end;
    int result_index;
};

struct AtlasFontHeader 
{
    float padding;
    float height;
    float ascend;
    float distslope;
    int numRanges;
    int numChars;
    char name[32];
};

struct SCREEN_AtlasFont 
{
    ~SCREEN_AtlasFont();

    float padding;
    float height;
    float ascend;
    float distslope;
    const AtlasChar *charData;
    const AtlasCharRange *ranges;
    int numRanges;
    int numChars;
    char name[32];

    // Returns 0 on no match.
    const AtlasChar *getChar(int utf32) const ;
};

struct AtlasHeader 
{
    int magic;
    int version;
    int numFonts;
    int numImages;
};

typedef std::vector<AtlasImage> SpriteSheetAllFrames;
struct SCREEN_SpriteSheet 
{
    std::string id;
    int numberOfFrames;
    SpriteSheetAllFrames frames;
};
typedef std::vector<SpriteSheet> SpriteSheetArray;

struct SCREEN_Atlas 
{
    SCREEN_Atlas() {}
    ~SCREEN_Atlas();
    
    bool Load(const uint8_t *data, size_t data_size);
    bool IsMetadataLoaded() 
    {
        return images != nullptr;
    }

    SCREEN_AtlasFont *fonts = nullptr;
    int num_fonts = 0;
    AtlasImage *images = nullptr;
    int num_images = 0;

    // These are inefficient linear searches, try not to call every frame.
    const SCREEN_AtlasFont *getFont(FontID id) const;
    const AtlasImage *getImage(SCREEN_ImageID id) const;
    bool registerSpriteSheet(std::string id, int numberOfFrames);

    bool measureImage(SCREEN_ImageID id, float *w, float *h) const;
    
    SpriteSheetArray sprite_sheets;
};
