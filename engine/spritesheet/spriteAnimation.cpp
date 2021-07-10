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

#include "engine.h"
#include "core.h"
#include "spriteAnimation.h"

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

void SpriteAnimation::Create(uint millisecondsPerFrame, SpriteSheet* spritesheet)
{
    Create(spritesheet->GetNumberOfSprites(), millisecondsPerFrame, spritesheet);
}

Sprite* SpriteAnimation::GetSprite()
{
    Sprite* sprite;
    if (IsRunning())
    {
        uint index = static_cast<int>((Engine::m_Engine->GetTime() - m_StartTime) * m_TimeFactor);
        sprite = m_Spritesheet->GetSprite(index);
    }
    else
    {
        sprite = m_Spritesheet->GetSprite(0);
    }
    return sprite;
}

void SpriteAnimation::Start()
{ 
    m_PreviousFrame = -1;
    m_StartTime = Engine::m_Engine->GetTime();
}

bool SpriteAnimation::IsRunning() const
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
