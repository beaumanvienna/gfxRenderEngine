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
#include "glm.hpp"

class Transformation
{

public:

    Transformation(float duration);
    void Start();
    bool IsRunning();

protected:

    float m_StartTime;
    float m_Duration;
    glm::mat4 m_Transformation;

};

class Translation : public Transformation
{

public:

    Translation(float duration /* in seconds */, float x1, float x2);
    glm::mat4& GetTransformation();

private:

    float m_X1, m_X2;

};

class Rotation : public Transformation
{

public:

    Rotation(float duration /* in seconds */, float rotation1, float rotation2);
    glm::mat4& GetTransformation();

private:

    float m_Rotation1, m_Rotation2;
    
};

class Scaling : public Transformation
{

public:

    Scaling(float duration /* in seconds */, float scale1, float scale2);
    glm::mat4& GetTransformation();

private:

    float m_Scale1, m_Scale2;

};

class Animation
{

public:

    Animation();
    void Start();
    void Reset();
    bool IsRunning();

    void AddTranslation(const Translation translation);
    void AddRotation(const Rotation rotation);
    void AddScaling(const Scaling scale);
    glm::mat4& GetTransformation();

private:

    std::vector<Translation> m_Translations;
    std::vector<Rotation> m_Rotations;
    std::vector<Scaling> m_Scalings;
    glm::mat4 m_Transformation;
    bool m_IsRunning;
    int m_CurrentSequenceTranslation;
    int m_CurrentSequenceRotation;
    int m_CurrentSequenceScale;
    
    int m_NumberOfTranslationSequences;
    int m_NumberOfRotationSequences;
    int m_NumberOfScaleSequences;

};
